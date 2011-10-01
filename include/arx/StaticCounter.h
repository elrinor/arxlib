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
#ifndef ARX_STATIC_COUNTER_H
#define ARX_STATIC_COUNTER_H

#include <arx/config.h>
#include <boost/preprocessor/cat.hpp>

/**
 * Defines a new static counter with initial value of 0. For a newly created counter ARX_GET_COUNTER will return 0. 
 *
 * @param id identifier for a new counter. Must be either a number or a well-formed c++ identifier.
 */
#define ARX_NEW_COUNTER(id)                                                     \
  template<int N> class BOOST_PP_CAT(counter, id):                              \
    public BOOST_PP_CAT(counter, id)<N - 1> {};                                 \
  template<> class BOOST_PP_CAT(counter, id)<0> {};                             \
  char (*arx_test_counter(const BOOST_PP_CAT(counter, id)<0> *))[1];

/**
 * Returns the value of a static counter identified by id.
 */
#define ARX_GET_COUNTER(id) (sizeof(*arx_test_counter(static_cast<BOOST_PP_CAT(counter, id)<200>*>(NULL))) - 1)

/**
 * Increments the value of a static counter identified by id.
 * Must be used in the same namespace where ARX_NEW_COUNTER was used.
 */
#define ARX_INC_COUNTER(id) char (*arx_test_counter(const BOOST_PP_CAT(counter, id)<ARX_GET_COUNTER(id) + 1>*))[ARX_GET_COUNTER(id) + 2];

#endif // ARX_STATIC_COUNTER_H
