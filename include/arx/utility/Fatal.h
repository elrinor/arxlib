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
#    define ARX_FATAL_ERROR(msg) _assert(msg, __FILE__, __LINE__)
#  elif defined(ARX_GCC)
#    define ARX_FATAL_ERROR(msg) __assert(__FILE__, __LINE__, msg)
#  else
#    define ARX_FATAL_ERROR(msg) std::abort()
#  endif
#else
#  define ARX_FATAL_ERROR(msg) assert(msg)
#endif

#define fatal() {                                                               \
    ARX_FATAL_ERROR("Unreachable code executed");                               \
    ARX_UNREACHABLE_CODE();                                                     \
  }

#define fatal_msg(msg) {                                                        \
    ARX_FATAL_ERROR(msg);                                                       \
    ARX_UNREACHABLE_CODE();                                                     \
  }

#endif // ARX_UTILITY_FATAL_H
