/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#ifndef ARX_MEMORY_H
#define ARX_MEMORY_H

#include <arx/config.h>
#include <cstdlib>
#include <cstddef>
#include <malloc.h>
#include <memory>
#include <boost/static_assert.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/has_new_operator.hpp>
#include "Utility.h"


/**
 * @def ALIGN
 * This attribute forces the data to be aligned.
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
    if(posix_memalign(&result, align, size) != 0)
      return NULL;
    else
      return result;
#elif defined(ARX_WIN32)
    return _aligned_malloc(size, align);
#else
    /* generic version built on top of malloc */
    if((align & (align - 1)) != 0)
      return NULL; /* not a power of 2 */
    if((align & (sizeof(void*) - 1)) != 0)
      return NULL; /* not multiple of sizeof(void*) */

    char* ptr = static_cast<char*>(malloc(size + align + sizeof(char*) - 1));
    char* p = reinterpret_cast<char*>(reinterpret_cast<intptr_t>(ptr + align - 1) & -align);
    if(p - ptr < sizeof(char*))
      p += align;
    *(reinterpret_cast<char**>(p) - 1) = ptr;
    
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
    free(*(static_cast<char**>(ptr) - 1));
#endif
  }


  /**
   * This class is provided for those who want to use aligned allocation with operator new.
   * If you want your class Foo to use aligned heap allocation, then just include WithAlignedOperatorNew in
   * the list of it's parents. Use public inheritance.
   *
   * @param align desired alignment, in bytes.
   */
  template<int align>
  struct WithAlignedOperatorNew {
    static_assert((align & (align - 1)) == 0, "Align must be a power of 2.");
    static_assert(align % sizeof(void*) == 0, "Align must be a multiple of sizeof(void*).");

    void* operator new(std::size_t size) throw() {
      return aligned_malloc(size, align);
    }

    void* operator new[](std::size_t size) throw() {
      return aligned_malloc(size, align);
    }

    void operator delete(void* ptr) { 
      aligned_free(ptr);
    }

    void operator delete[](void* ptr) { 
      aligned_free(ptr);
    }
  };


// -------------------------------------------------------------------------- //
// classnew_allocator
// -------------------------------------------------------------------------- //
  /** Replacement for std::allocator that calls overloaded operator new if one
   * is present. */
  template<class T> class classnew_allocator {
  public:
    typedef T         value_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef typename std::allocator<T>::size_type         size_type;
    typedef typename std::allocator<T>::difference_type   difference_type;

    template<typename OtherType>
    struct rebind {
      typedef classnew_allocator<OtherType> other; 
    };

    classnew_allocator() {}

    template<class Other>
    classnew_allocator(const classnew_allocator<Other>&) {}

    pointer address(reference ref) const { 
      return addressof(ref); 
    }

    const_pointer address(const_reference ref) const { 
      return addressof(ref); 
    }

    pointer allocate(size_type size, const void* hint = 0) { 
      return impl_type::allocate(size, hint);
    }

    void deallocate(pointer ptr, size_type size) { 
      impl_type::deallocate(ptr, size);
    }
    
    size_type max_size() const { 
      return size_type(-1) / sizeof(T); 
    }

    void construct(pointer ptr, const value_type& refObj) { 
      ::new(ptr) T(refObj); 
    }
    
    void destroy(pointer ptr) {
      (void) ptr; /* This one fixes a bogus MSVC warning (C4100: 'ptr' unreferenced formal parameter). */

      ptr->~T(); 
    }

    template<class OtherType>
    bool operator==(const classnew_allocator<OtherType>&) {
      return impl_type::HAS_NEW == classnew_allocator<OtherType>::impl_type::HAS_NEW;
    }

    template<class OtherType>
    bool operator!=(const classnew_allocator<OtherType>& other) {
      return !operator==(other);
    }

  private:
    template<class U, bool has_new = boost::has_new_operator<U>::value> 
    struct allocator_impl {
      enum { HAS_NEW = true };

      static pointer allocate(size_t size, const void* /* hint */ = 0) { 
        return static_cast<pointer>(U::operator new(size * sizeof(U)));
      }

      static void deallocate(pointer ptr, size_t) { 
        U::operator delete(ptr); 
      }
    };

    template<class U> 
    struct allocator_impl<U, false> {
      enum { HAS_NEW = false };

      static pointer allocate(size_t size, const void* = 0) {
        return static_cast<pointer>(::operator new(size * sizeof(U))); 
      }

      static void deallocate(pointer ptr, size_t) { 
        ::operator delete(ptr);
      }
    };

    typedef allocator_impl<T> impl_type;
  };

} // namespace arx

#endif // ARX_MEMORY_H
