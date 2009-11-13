#ifndef __ARX_STATICBLOCK_H__
#define __ARX_STATICBLOCK_H__

#include <boost/preprocessor/cat.hpp>

#define ARX_STATIC_BLOCK_I(BLOCK_NAME)                                          \
  template<class T> class BLOCK_NAME {                                          \
  private:                                                                      \
    typedef T tag;                                                              \
                                                                                \
    static bool mValue;                                                         \
                                                                                \
    static bool init() {                                                        \
      block();                                                                  \
      return true;                                                              \
    }                                                                           \
                                                                                \
    static void block();                                                        \
  };                                                                            \
                                                                                \
  template<class T> bool BLOCK_NAME<T>::mValue = BLOCK_NAME<T>::init();         \
                                                                                \
  template class BLOCK_NAME<void>; /* Explicitly instatiate it. */              \
                                                                                \
  template<class T> void BLOCK_NAME<T>::block()                                 \

/**
 * Defines a java-like static block of code. Can be used in .h files safely - the
 * code will be executed only once regardless of the number of times
 * the .h file was included.
 *
 * @param FILE_ID unique identifier used to prevent name clashes.
 */
#define ARX_STATIC_BLOCK(FILE_ID)                                               \
  ARX_STATIC_BLOCK_I(BOOST_PP_CAT(StaticBlock, BOOST_PP_CAT(FILE_ID, __LINE__)))

#endif // __ARX_STATICBLOCK_H__
