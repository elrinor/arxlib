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
#ifndef ARX_FOREACH_H
#define ARX_FOREACH_H

#include <boost/foreach.hpp>
#include <boost/preprocessor/cat.hpp>

/* Undefine qt foreach. */
#ifdef foreach
#  undef foreach
#endif

#define foreach BOOST_FOREACH

#define reverse_foreach BOOST_REVERSE_FOREACH

#define ARX_FOREACH_VAR(NAME) BOOST_PP_CAT(arx_foreach_, BOOST_PP_CAT(NAME, __LINE__))

#define map_foreach(KEY, VAL, MAP)                                              \
  if(bool ARX_FOREACH_VAR(_continue) = true)                                    \
  foreach(auto pair, MAP)                                                       \
  if(!ARX_FOREACH_VAR(_continue)) break; else                                   \
  if(bool ARX_FOREACH_VAR(_iteration) = true)                                   \
  if((ARX_FOREACH_VAR(_continue) = false), true)                                \
  for(KEY = pair.first; ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_iteration) = false) \
  for(VAL = pair.second; ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_continue) = true, ARX_FOREACH_VAR(_iteration) = false)

#endif // ARX_FOREACH_H
