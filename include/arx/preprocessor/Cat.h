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
#ifndef ARX_PREPROCESSOR_CAT_H
#define ARX_PREPROCESSOR_CAT_H

#include <arx/config.h>
#include <boost/preprocessor/cat.hpp>

#define ARX_CAT BOOST_PP_CAT
#define ARX_CAT_3(a, b, c) BOOST_PP_CAT(BOOST_PP_CAT(a, b), c)
#define ARX_CAT_4(a, b, c, d) BOOST_PP_CAT(BOOST_PP_CAT(a, b), BOOST_PP_CAT(c, d))
#define ARX_CAT_5(a, b, c, d, e) BOOST_PP_CAT(BOOST_PP_CAT(a, b), BOOST_PP_CAT(BOOST_PP_CAT(c, d), e))

#endif // ARX_PREPROCESSOR_CAT_H

