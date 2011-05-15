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
#ifndef ARX_UTILITY_UNREACHABLE_H
#define ARX_UTILITY_UNREACHABLE_H

#include <arx/config.h>
#include <cassert>

#ifdef ARX_MSVC
#  define ARX_UNREACHABLE_CODE() __assume(false)
#elif defined(ARX_GCC) && (ARX_GCC >= 40500)
#  define ARX_UNREACHABLE_CODE() __builtin_unreachable()
#else 
#  /* We cannot define ARX_UNREACHABLE_CODE() as empty since it will generate 
#   * "Control reaches end of non-void function" warnings. */
#  define ARX_UNREACHABLE_CODE() for(;;) {} 
#endif

/**
 * Marks code as unreachable. 
 * 
 * Produces assertion in debug builds and emits compiler-specific optimization
 * hints.
 */
#define unreachable() {                                                         \
    assert(!"Unreachable code executed.");                                      \
    ARX_UNREACHABLE_CODE();                                                     \
  }

#ifdef ARX_NO_DEPRECATE
/**
 * Compatibility definition.
 * 
 * @see unreachable
 */
#  define Unreachable unreachable
#endif

#endif // ARX_UTILITY_UNREACHABLE_H
