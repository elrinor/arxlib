#ifndef __ARX_MEMORY_H__
#define __ARX_MEMORY_H__

#include "config.h"
#include <malloc.h>
#include <cstdlib>
#include <cstddef>
#include "static_assert.h"

/**
 * @def ALIGN
 * This attribyte forces the data to be aligned.
 *
 * @param BYTES alignment.
 */
#if defined(ARX_GCC)
#  define ALIGN(BYTES) __attribute__((aligned(BYTES)))
#elif defined(ARX_MSVC) || defined(ARX_ICC)
#  define ALIGN(BYTES) __declspec(align(BYTES))
#else
#  /* Not supported. */
#endif

/**
 * @def ALIGNOF
 *
 * @returns alignment in bytes of the given type.
 */
#if defined(ARX_MSVC) || defined(ARX_ICC) || defined(ARX_GCC)
#  define ALIGNOF(TYPE) __alignof(TYPE)
#else
#  /* Not supported. */
#endif

#ifdef ARX_LINUX
/* posix_memalign isn't always defined in the stdlib header */
extern "C" int posix_memalign(void**, size_t, size_t) throw ();
#endif

namespace arx {

  /**
   * Allocates an aligned chunk of memory.
   *
   * @param size size of memory chunk, in bytes
   * @param align alignment, in bytes
   * @returns pointer to a newly allocated aligned chunk of memory, or NULL in case of failure
   */
  inline void* aligned_malloc(std::size_t size, int align) {
#if defined(ARX_LINUX)
    void* result;
    if (posix_memalign(&result, align, size) != 0)
      return NULL;
    else
      return result;
#elif defined(ARX_WIN32)
    return _aligned_malloc(size, align);
#else
    /* generic version built on top of malloc */
    if ((align & (align - 1)) != 0)
      return NULL; /* not a power of 2 */
    if ((align & (sizeof(void*) - 1)) != 0)
      return NULL; /* not multiple of sizeof(void*) */

    char* ptr = static_cast<char*>(malloc(size + align + sizeof(char*) - 1));
    char* p = reinterpret_cast<char*>(reinterpret_cast<intptr_t>(ptr + align - 1) & -align);
    if(p - ptr < sizeof(char*))
      p += align;
    *(static_cast<char **>(p) - 1) = ptr;
    
    return p;
#endif
  }


  /** 
   * Frees memory allocated with aligned_malloc. Using this function for releasing memory 
   * not allocated by aligned_malloc may result in undefined behavior. Calling this function
   * for NULL pointer results in no action.
   *
   * @param ptr pointer to a memory block to free
   */
  inline void aligned_free(void* ptr) {
#if defined(ARX_LINUX)
    free(ptr);
#elif defined(ARX_WIN32)
    _aligned_free(ptr);
#else
    if(ptr == NULL)
      return;
    free(*(static_cast<char **>(ptr) - 1));
#endif
  }


  /**
   * This class is provided for those who want to use aligned allocation with operator new.
   * If you want your class Foo to use aligned heap allocation, then just include WithAlignedOperatorNew in
   * the list of it's parents. Private inheritance is OK.
   *
   * @param align desired alignment, in bytes.
   */
  template<int align>
  struct WithAlignedOperatorNew {
    STATIC_ASSERT(((align & (align - 1)) == 0)); /* is a power of 2 */
    STATIC_ASSERT(((align & (sizeof(void*) - 1)) == 0)); /* is a multiple of sizeof(void*) */

    void* operator new(std::size_t size) throw() {
      return aligned_malloc(size, align);
    }

    void *operator new[](std::size_t size) throw() {
      return aligned_malloc(size, align);
    }

    void operator delete(void* ptr) { 
      aligned_free(ptr);
    }

    void operator delete[](void* ptr) { 
      aligned_free(ptr); 
    }
  };

} // namespace arx

#endif // __ARX_MEMORY_H__
