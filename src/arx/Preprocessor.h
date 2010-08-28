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
#ifndef ARX_PREPROCESSOR_H
#define ARX_PREPROCESSOR_H

#include "config.h"
#include <boost/preprocessor/cat.hpp>

#include <boost/preprocessor/for.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#define ARX_STRIP_I(...) __VA_ARGS__
#define ARX_STRIP(ARG) ARX_STRIP_I ARG

#define ARX_CAT BOOST_PP_CAT
#define ARX_CAT_3(a, b, c) BOOST_PP_CAT(BOOST_PP_CAT(a, b), c)
#define ARX_CAT_4(a, b, c, d) BOOST_PP_CAT(BOOST_PP_CAT(a, b), BOOST_PP_CAT(c, d))


#define ARX_NUMERIC_FOR_OP(R, STATE)                                            \
  (                                                                             \
    BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(4, 0, STATE)),                             \
    BOOST_PP_TUPLE_ELEM(4, 1, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 2, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 3, STATE)                                            \
  )

#define ARX_NUMERIC_FOR_PRED(R, STATE)                                          \
  BOOST_PP_NOT_EQUAL(                                                           \
    BOOST_PP_TUPLE_ELEM(4, 0, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 1, STATE)                                            \
  )

#define ARX_NUMERIC_FOR_MACRO(R, STATE)                                         \
  BOOST_PP_TUPLE_ELEM(4, 3, STATE)(R, BOOST_PP_TUPLE_ELEM(4, 2, STATE), BOOST_PP_TUPLE_ELEM(4, 0, STATE))

/**
 * Performs a horizontal repetition that can be best described with the
 * following pseudocode:
 *
 * for(int i = MIN; i != MAX; i++)
 *   MACRO(R, DATA, i);
 */
#define ARX_NUMERIC_FOR(MIN, MAX, DATA, MACRO)                                  \
  BOOST_PP_FOR((MIN, MAX, DATA, MACRO), ARX_NUMERIC_FOR_PRED, ARX_NUMERIC_FOR_OP, ARX_NUMERIC_FOR_MACRO)
  

#endif // ARX_PREPROCESSOR_H
