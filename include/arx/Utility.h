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
#ifndef ARX_UTILITY_H
#define ARX_UTILITY_H

#include "config.h"
#include "utility/Fatal.h"
#include "utility/UnorderedPair.h"
#include "utility/Unreachable.h"
#include "utility/Unused.h"
#include "utility/Verify.h"

// -------------------------------------------------------------------------- //
// Some useful defines
// -------------------------------------------------------------------------- //
#ifdef ARX_MSVC
#  define ALWAYS_INLINE __forceinline
#elif defined(ARX_GCC)
#  define ALWAYS_INLINE inline __attribute__((__always_inline__))
#else
#  define ALWAYS_INLINE inline
#endif

#ifdef ARX_MSVC
#  define NEVER_INLINE __declspec(noinline)
#elif defined(ARX_GCC)
#  define NEVER_INLINE __attribute__((__noinline__))
#else
#  define NEVER_INLINE
#endif

#ifdef ARX_MSVC
#  define NORETURN __declspec(noreturn)
#elif defined(ARX_GCC)
#  define NORETURN __attribute((__noreturn__))
#else
#  define NORETURN
#endif

namespace arx {
// -------------------------------------------------------------------------- //
// Some useful functions
// -------------------------------------------------------------------------- //
  /**
   * Square template.
   */
  template<class T> 
  inline T sqr(T x) {
    return x * x;
  }

} // namespace arx

#endif // ARX_UTILITY_H
