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
#ifndef ARX_RANGE_MAPPED_VALUE_H
#define ARX_RANGE_MAPPED_VALUE_H

#include <arx/config.h>
#include <boost/range/value_type.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/end.hpp>
#include "Find.h"

namespace arx {

  template<class Range, class Key, class Value>
  typename boost::range_value<const Range>::type::second_type
  mapped_value(const Range &range, const Key &key, const Value &defaultValue) {
    typename boost::range_iterator<const Range>::type pos = arx::find(range, key);

    return pos == boost::end(range) ? defaultValue : pos->second;
  }

  template<class Range, class Key>
  typename boost::range_value<const Range>::type::second_type
  mapped_value(const Range &range, const Key &key) {
    typename boost::range_iterator<const Range>::type pos = arx::find(range, key);

    return pos == boost::end(range) ? typename boost::range_value<const Range>::type::second_type() : pos->second;
  }

} // namespace arx

#endif // ARX_RANGE_MAPPED_VALUE_H
