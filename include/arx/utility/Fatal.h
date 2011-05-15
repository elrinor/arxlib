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
#ifndef ARX_UTILITY_FATAL_H
#define ARX_UTILITY_FATAL_H

#include <arx/config.h>
#include <boost/preprocessor/cat.hpp>
#include <cassert>
#include "Unreachable.h"

#ifdef NDEBUG
#  if defined(ARX_MSVC)
#    define fatal() ((void) _wassert(L"Unreachable code executed", BOOST_PP_CAT(L, __FILE__), __LINE__))
#  elif defined(ARX_GCC)
#    define fatal() ((void) __assert(__FILE__, __LINE__, "Unreachable code executed"))
#  else
#    define fatal() ((void) std::abort())
#  endif
#else
#  define fatal() assert(!"Unreachable code executed")
#endif

#endif // ARX_UTILITY_FATAL_H
