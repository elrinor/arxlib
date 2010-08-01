#ifndef ARX_STATIC_BLOCK_H
#define ARX_STATIC_BLOCK_H

#include <boost/preprocessor/cat.hpp>

#define ARX_STATIC_BLOCK_I(BLOCK_NAME)                                          \
  template<class T> class BLOCK_NAME {                                          \
  public:                                                                       \
    BLOCK_NAME() {                                                              \
      if(!sExecuted) {                                                          \
        block();                                                                \
        sExecuted = true;                                                       \
      }                                                                         \
    }                                                                           \
                                                                                \
  private:                                                                      \
    static inline void block();                                                 \
                                                                                \
    static bool sExecuted;                                                      \
  };                                                                            \
                                                                                \
  template<> bool BLOCK_NAME<void>::sExecuted = false;                          \
                                                                                \
  namespace {                                                                   \
    BLOCK_NAME<void> BOOST_PP_CAT(BLOCK_NAME, _value);                          \
  }                                                                             \
                                                                                \
  template<> void BLOCK_NAME<void>::block()                                     \

/**
 * Defines a java-like static block of code. 
 * 
 * It can safely be used in .h files - the code will be executed only once 
 * regardless of the number of translation units the .h file was included into.
 *
 * It is guaranteed that static block will be executed BEFORE any static
 * initializers that follow it in every translation unit.
 *
 * @param FILE_ID unique identifier used to prevent name clashes.
 */
#define ARX_STATIC_BLOCK(FILE_ID)                                               \
  ARX_STATIC_BLOCK_I(BOOST_PP_CAT(StaticBlock, BOOST_PP_CAT(FILE_ID, __LINE__)))

/**
 * @see ARX_STATIC_BLOCK
 */
#define ARX_STATIC_INITIALIZER(FILE_ID)                                         \
  ARX_STATIC_BLOCK(FILE_ID)

#define ARX_STATIC_DEINITIALIZER_I(BLOCK_NAME)                                  \
  template<class T> class BLOCK_NAME {                                          \
  public:                                                                       \
    BLOCK_NAME() {                                                              \
      sCounter++;                                                               \
    }                                                                           \
                                                                                \
    ~BLOCK_NAME() {                                                             \
      sCounter--;                                                               \
      if(sCounter == 0)                                                         \
        block();                                                                \
    }                                                                           \
                                                                                \
  private:                                                                      \
    static inline void block();                                                 \
                                                                                \
    static int sCounter;                                                        \
  };                                                                            \
                                                                                \
  template<> int BLOCK_NAME<void>::sCounter = 0;                                \
                                                                                \
  namespace {                                                                   \
    BLOCK_NAME<void> BOOST_PP_CAT(BLOCK_NAME, _value);                          \
  }                                                                             \
                                                                                \
  template<> void BLOCK_NAME<void>::block()

/**
 * Defines a deinitialization block of code.
 *
 * It can be safely used in .h files - the code will be executed only once,
 * regardless if the number of translation units the .h file was included into.
 *
 * It is guaranteed that deinitialization block will be executed AFTER all
 * static initializers that follow it in all translation units.
 *
 * @param FILE_ID unique identifier used to prevent name clashes.
 */
#define ARX_STATIC_DEINITIALIZER(FILE_ID)                                       \
  ARX_STATIC_DEINITIALIZER_I(BOOST_PP_CAT(StaticDeinitializer, BOOST_PP_CAT(FILE_ID, __LINE__)))

#endif // ARX_STATIC_BLOCK_H
