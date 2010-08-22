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
#ifndef ARX_HASH_HACK_H
#define ARX_HASH_HACK_H

#include <typeindex>

/* Boost doesn't provide a hash function for std::type_index, so we have to do 
 * in ourselves. 
 *
 * We're injecting code into std namespace here, which is bad. */
namespace std {
  size_t hash_value(const type_index& typeIndex) {
    return hash<type_index>()(typeIndex);
  }

} // namespace std

#endif // ARX_HASH_HACK_H
