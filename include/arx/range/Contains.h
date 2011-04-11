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
#ifndef ARX_RANGE_CONTAINS_H
#define ARX_RANGE_CONTAINS_H

#include "config.h"
#include <algorithm> /* For std::find. */
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "HasXxx.h"

namespace arx {
  namespace range_protected {
    ARX_DEFINE_HAS_FUNC_TRAIT(find);

    template<class AssociativeContainer, class T, bool hasFind = has_find<AssociativeContainer>::value>
    struct Contains {
      bool operator() (const AssociativeContainer &container, const T &value) {
        return container.find(value) != container.end();
      }
    };

    template<class Range, class T>
    struct Contains<Range, T, false> {
      bool operator() (const Range &range, const T &value) {
        return std::find(boost::begin(range), boost::end(range), value) != boost::end(range);
      }
    };

  } // namespace range_protected

  template<class Range, class T>
  bool contains(const Range &range, const T &value) {
    return range_protected::Contains<Range, T>()(range, value);
  }

} // namespace arx

#endif // ARX_RANGE_CONTAINS_H
