#ifndef ARX_STATIC_BLOCK_H
#define ARX_STATIC_BLOCK_H

#include <boost/preprocessor/cat.hpp>

#define ARX_STATIC_BLOCK_I(BLOCK_NAME, DERIVED_BLOCK_NAME)                      \
  class BLOCK_NAME {                                                            \
  protected:                                                                    \
    BLOCK_NAME() {}                                                             \
                                                                                \
    static inline void block();                                                 \
  };                                                                            \
                                                                                \
  template<class T> class DERIVED_BLOCK_NAME: private BLOCK_NAME {              \
  public:                                                                       \
    DERIVED_BLOCK_NAME() {                                                      \
      if(!sExecuted) {                                                          \
        block();                                                                \
        sExecuted = true;                                                       \
      }                                                                         \
    }                                                                           \
                                                                                \
  private:                                                                      \
    static bool sExecuted;                                                      \
  };                                                                            \
                                                                                \
  template<> bool DERIVED_BLOCK_NAME<void>::sExecuted = false;                  \
                                                                                \
  namespace {                                                                   \
    DERIVED_BLOCK_NAME<void> BOOST_PP_CAT(BLOCK_NAME, _value);                  \
  }                                                                             \
                                                                                \
  void BLOCK_NAME::block()


/**
 * Defines a java-like static block of code that resides in a class with the
 * given name. Client code can befriend this class.
 *
 * @see ARX_STATIC_BLOCK
 */
#define ARX_NAMED_STATIC_BLOCK(BLOCK_NAME)                                      \
  ARX_STATIC_BLOCK_I(BLOCK_NAME, BOOST_PP_CAT(BLOCK_NAME, _DERIVED))


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
  ARX_NAMED_STATIC_BLOCK(BOOST_PP_CAT(StaticBlock, BOOST_PP_CAT(FILE_ID, __LINE__)))


/**
 * @see ARX_NAMED_STATIC_BLOCK
 */ 
#define ARX_NAMED_STATIC_INITIALIZER ARX_NAMED_STATIC_BLOCK


/**
 * @see ARX_STATIC_BLOCK
 */
#define ARX_STATIC_INITIALIZER ARX_STATIC_BLOCK


#define ARX_STATIC_DEINITIALIZER_I(BLOCK_NAME, DERIVED_BLOCK_NAME)              \
  class BLOCK_NAME {                                                            \
  protected:                                                                    \
    BLOCK_NAME() {}                                                             \
                                                                                \
    static inline void block();                                                 \
  };                                                                            \
                                                                                \
  template<class T> class DERIVED_BLOCK_NAME: private BLOCK_NAME {              \
  public:                                                                       \
    DERIVED_BLOCK_NAME() {                                                      \
      sCounter++;                                                               \
    }                                                                           \
                                                                                \
    ~DERIVED_BLOCK_NAME() {                                                     \
      sCounter--;                                                               \
      if(sCounter == 0)                                                         \
        block();                                                                \
    }                                                                           \
                                                                                \
  private:                                                                      \
    static int sCounter;                                                        \
  };                                                                            \
                                                                                \
  template<> int DERIVED_BLOCK_NAME<void>::sCounter = 0;                        \
                                                                                \
  namespace {                                                                   \
    DERIVED_BLOCK_NAME<void> BOOST_PP_CAT(BLOCK_NAME, _value);                  \
  }                                                                             \
                                                                                \
  void BLOCK_NAME::block()


/**
 * Defines a deinitialization block of code that resides in a class with the
 * given name. Client code can befriend this class.
 *
 * @see ARX_STATIC_DEINITIALIZER
 */
#define ARX_NAMED_STATIC_DEINITIALIZER(CLASS_NAME)                              \
  ARX_STATIC_DEINITIALIZER_I(CLASS_NAME, BOOST_PP_CAT(CLASS_NAME, _DERIVED))


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
  ARX_NAMED_STATIC_DEINITIALIZER(BOOST_PP_CAT(StaticDeinitializer, BOOST_PP_CAT(FILE_ID, __LINE__)))


#define ARX_STATIC_VARIABLE_I(TYPE, NAME, VALUE, CLASS_NAME)                    \
  template<class T>                                                             \
  struct CLASS_NAME {                                                           \
    static TYPE NAME;                                                           \
  };                                                                            \
                                                                                \
  template<class T> TYPE CLASS_NAME<T>::NAME = VALUE;                           \
                                                                                \
  namespace {                                                                   \
    TYPE &NAME = CLASS_NAME<TYPE>::NAME;                                        \
  }


/**
 * Defines a static variable of the given type. This macro can be safely used
 * in header files without the need to explicitly allocate memory for the
 * variable in the source.
 *
 * Note that created static variable has no protection against static
 * initialization order fiasco.
 *
 * @param TYPE                         variable type.
 * @param NAME                         variable name.
 * @param VALUE                        initial variable value.
 */
#define ARX_STATIC_VARIABLE(TYPE, NAME, VALUE)                                  \
  ARX_STATIC_VARIABLE_I(TYPE, NAME, VALUE, BOOST_PP_CAT(BOOST_PP_CAT(NAME, _holder_), __LINE__))

#endif // ARX_STATIC_BLOCK_H
