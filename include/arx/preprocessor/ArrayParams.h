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
#ifndef ARX_PREPROCESSOR_ARRAY_PARAMS_H
#define ARX_PREPROCESSOR_ARRAY_PARAMS_H

#include "config.h"
#include <boost/preprocessor/array/elem.hpp>
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/seq/to_array.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/arithmetic/div.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/comma_if.hpp>

#define ARX_ARRAY_BINARY_PARAMS_M(z, i, data)                                   \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0))                                   \
  BOOST_PP_ARRAY_ELEM(BOOST_PP_MUL(i, 2), data)                                 \
  BOOST_PP_ARRAY_ELEM(BOOST_PP_INC(BOOST_PP_MUL(i, 2)), data)

#define ARX_ARRAY_BINARY_PARAMS(param_array)                                    \
  BOOST_PP_REPEAT(BOOST_PP_DIV(BOOST_PP_ARRAY_SIZE(param_array), 2), ARX_ARRAY_BINARY_PARAMS_M, param_array)

#define ARX_ARRAY_TRAILING_BINARY_PARAMS(param_array)                           \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_ARRAY_SIZE(param_array), 0))    \
  ARX_ARRAY_BINARY_PARAMS(param_array)


#define ARX_ARRAY_NTH_BINARY_PARAMS_M(z, i, data)                               \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0))                                   \
  BOOST_PP_ARRAY_ELEM(                                                          \
  BOOST_PP_ADD(BOOST_PP_MUL(i, 2), BOOST_PP_TUPLE_ELEM(2, 0, data)),          \
  BOOST_PP_TUPLE_ELEM(2, 1, data)                                             \
  )

#define ARX_ARRAY_NTH_BINARY_PARAMS(n, param_array)                             \
  BOOST_PP_REPEAT(BOOST_PP_DIV(BOOST_PP_ARRAY_SIZE(param_array), 2), ARX_ARRAY_NTH_BINARY_PARAMS_M, (n, param_array))

#define ARX_ARRAY_TRAILING_NTH_BINARY_PARAMS(n, param_array)                    \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_ARRAY_SIZE(param_array), 0))    \
  ARX_ARRAY_NTH_BINARY_PARAMS(n, param_array)


#define ARX_ARRAY_LEFT_BINARY_PARAMS(param_array)                               \
  ARX_ARRAY_NTH_BINARY_PARAMS(0, param_array)

#define ARX_ARRAY_TRAILING_LEFT_BINARY_PARAMS(param_array)                      \
  ARX_ARRAY_TRAILING_NTH_BINARY_PARAMS(0, param_array)


#define ARX_ARRAY_RIGHT_BINARY_PARAMS(param_array)                              \
  ARX_ARRAY_NTH_BINARY_PARAMS(1, param_array)

#define ARX_ARRAY_TRAILING_RIGHT_BINARY_PARAMS(param_array)                     \
  ARX_ARRAY_TRAILING_NTH_BINARY_PARAMS(1, param_array)


#define ARX_ARRAY_PARAMS_M(z, i, param_array)                                   \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(i, 0))                                   \
  BOOST_PP_ARRAY_ELEM(i, param_array)

#define ARX_ARRAY_PARAMS(param_array)                                           \
  BOOST_PP_REPEAT(BOOST_PP_ARRAY_SIZE(param_array), ARX_ARRAY_PARAMS_M, param_array)

#define ARX_ARRAY_TRAILING_PARAMS(param_array)                                  \
  BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_ARRAY_SIZE(param_array), 0))    \
  ARX_ARRAY_PARAMS(param_array)

#endif // ARX_PREPROCESSOR_ARRAY_PARAMS_H
