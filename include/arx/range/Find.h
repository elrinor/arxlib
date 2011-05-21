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
#ifndef ARX_RANGE_FIND_H
#define ARX_RANGE_FIND_H

#include <arx/config.h>
#include <boost/mpl/bool.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/type_traits/detail/yes_no_type.hpp>

namespace arx {
  namespace detail {

    boost::type_traits::no_type is_associative(...);

    template<class Range>
    boost::type_traits::yes_type is_associative(const Range &, typename Range::key_type * = NULL);

    /**
     * Default implementation.
     */
    template<class Range, class T>
    typename boost::range_iterator<Range>::type
    range_find(Range &range, const T &value, boost::mpl::bool_<false>) {
      return boost::find(range, value);
    }

    /**
     * Implementation for associative containers.
     */
    template<class Range, class T>
    typename boost::range_iterator<Range>::type
    range_find(Range &range, const T &value, boost::mpl::bool_<true>) {
      return range.find(value);
    }

    template<class Range, class T>
    typename boost::range_iterator<Range>::type
    range_find(Range &range, const T &value) {
      return range_find(range, value, boost::mpl::bool_<sizeof(is_associative(range)) == sizeof(boost::type_traits::yes_type)>());
    }

  } // namespace detail

  template<class Range, class T>
  typename boost::range_iterator<Range>::type
  find(Range &range, const T &value) {
    using detail::range_find;

    return range_find(range, value);
  }

  template<class Range, class T>
  typename boost::range_iterator<const Range>::type
  find(const Range &range, const T &value) {
      using detail::range_find;

      return range_find(range, value);
  }

} // namespace arx

#endif // ARX_RANGE_FIND_H
