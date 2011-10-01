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
#ifndef ARX_RANGE_INSERT_H
#define ARX_RANGE_INSERT_H

#include <arx/config.h>
#include <boost/range/iterator.hpp>
#include <utility> /* For std::forward. */

namespace arx {
  namespace detail {
    template<class Range, class Element>
    void range_insert(Range &range, const typename boost::range_iterator<Range>::type &pos, Element &&element) {
      range.insert(pos, std::forward<Element>(element));
    }

  } // namespace detail

  /**
   * Inserts the given element into the given range at the given position.
   * 
   * @param range                      Range to insert given element into.
   * @param pos                        Position to insert at.
   * @param element                    Element to insert.
   */
  template<class Range, class Element>
  void insert(Range &range, const typename boost::range_iterator<Range>::type &pos, Element &&element) {
    using detail::range_insert;

    range_insert(range, pos, std::forward<Element>(element));
  }


} // namespace arx

#endif // ARX_RANGE_INSERT_H
