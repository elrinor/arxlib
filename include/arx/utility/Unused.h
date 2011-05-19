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
#ifndef ARX_UTILITY_UNUSED_H
#define ARX_UTILITY_UNUSED_H

#include <arx/config.h>

namespace arx { namespace detail {
  template<class T>
  void use(const T &) {}
}} // namespace arx::detail

/**
 * Macro to suppress unused parameter warnings. Unlike constructions like 
 * <tt>Q_UNUSED(X)</tt> or <tt>(void)x</tt>, does not require the parameter
 * to have a complete type.
 * 
 * @param X                            Name of the parameter to mark as 
 *                                     intentionally unused.
 */
#define ARX_UNUSED(X) ::arx::detail::use(X)

#ifndef ARX_NO_KEYWORD_UNUSED
#  define unused ARX_UNUSED
#endif

#endif // ARX_UTILITY_UNUSED_H