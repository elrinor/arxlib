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
 * @param CLASS_NAME                   Name of the class to create code 
 *                                     block in.
 * @see ARX_STATIC_BLOCK
 */
#define ARX_NAMED_STATIC_BLOCK(CLASS_NAME)                                      \
  ARX_STATIC_BLOCK_I(CLASS_NAME, BOOST_PP_CAT(BLOCK_NAME, _DERIVED))


/**
 * Defines a java-like static block of code. 
 * 
 * It can safely be used in header files - the code will be executed only once 
 * regardless of the number of translation units the header file was included 
 * into.
 *
 * It is guaranteed that static block will be executed BEFORE any static
 * initializers that follow it in every translation unit.
 *
 * @param FILE_ID                      unique identifier used to prevent name 
 *                                     clashes.
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
 * @param CLASS_NAME                   Name of the class to create code 
 *                                     block in.
 * @see ARX_STATIC_DEINITIALIZER
 */
#define ARX_NAMED_STATIC_DEINITIALIZER(CLASS_NAME)                              \
  ARX_STATIC_DEINITIALIZER_I(CLASS_NAME, BOOST_PP_CAT(CLASS_NAME, _DERIVED))


/**
 * Defines a deinitialization block of code.
 *
 * It can be safely used in header files - the code will be executed only once,
 * regardless of the number of translation units the header file was included 
 * into.
 *
 * It is guaranteed that deinitialization block will be executed AFTER all
 * static deinitializers that follow it in all translation units.
 *
 * @param FILE_ID                      unique identifier used to prevent name 
 *                                     clashes.
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
 * in header files - only one variable will be generated, regardless of the 
 * number of translation units the header file was included into.
 *
 * Note that created static variable has no protection against static
 * initialization order fiasco, i.e. in some translation units it may be
 * initialized after some of the static variables that follow it.
 *
 * @param TYPE                         Variable type.
 * @param NAME                         Variable name.
 * @param VALUE                        Initial variable value.
 */
#define ARX_STATIC_VARIABLE(TYPE, NAME, VALUE)                                  \
  ARX_STATIC_VARIABLE_I(TYPE, NAME, VALUE, BOOST_PP_CAT(BOOST_PP_CAT(NAME, _holder_), __LINE__))

#endif // ARX_STATIC_BLOCK_H
