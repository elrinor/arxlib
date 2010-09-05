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
#ifndef ARX_EXT_QT_MAP_FOREACH_H
#define ARX_EXT_QT_MAP_FOREACH_H

#include "config.h"
#include <arx/Foreach.h>

namespace arx { namespace detail {
  template<class T>
  class Holder {
  public:
    Holder(T &value): mValue(value) {}

    T &value() const {
      return mValue;
    }

    operator bool() const {
      return true;
    }

  private:
    T &mValue;
  };

  template<class T>
  Holder<T> create_holder(T &value) {
    return Holder<T>(value);
  }

  template<class T>
  Holder<const T> create_holder(const T &value) {
    return Holder<const T>(value);
  }

}} // namespace arx::detail

#define qmap_foreach(KEY, VAL, MAP)                                             \
  if(bool ARX_FOREACH_VAR(_continue) = true)                                    \
  if(auto ARX_FOREACH_VAR(holder) = arx::detail::create_holder(MAP))            \
  for(auto i = ARX_FOREACH_VAR(holder).value().begin(); i != ARX_FOREACH_VAR(holder).value().end(); i++) \
  if(!ARX_FOREACH_VAR(_continue)) break; else                                   \
  if(bool ARX_FOREACH_VAR(_iteration) = true)                                   \
  if((ARX_FOREACH_VAR(_continue) = false), true)                                \
  for(KEY = i.key(); ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_iteration) = false) \
  for(VAL = i.value(); ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_continue) = true, ARX_FOREACH_VAR(_iteration) = false)

#endif // ARX_EXT_QT_MAP_FOREACH_H
