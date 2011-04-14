/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
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
#ifndef ARX_EXT_BOOST_TEST_H
#define ARX_EXT_BOOST_TEST_H

#ifndef BOOST_TEST_MODULE
#  error BOOST_TEST_MODULE must be defined before including this header
#endif

/* Protect ourselves against nasty MS macros. */
#define NOMINMAX

/* Workaround for mingw not defining putenv in C++0x mode. */
#if defined(__MINGW32__) && defined(__STRICT_ANSI__)
#  include <stdlib.h>
extern "C" _CRTIMP int __cdecl __MINGW_NOTHROW	putenv (const char*);
#endif

#include <boost/test/included/unit_test.hpp>

#endif // ARX_EXT_BOOST_TEST_H_
