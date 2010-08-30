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
#ifndef ARX_XML_CHECKERS_H
#define ARX_XML_CHECKERS_H

#include "config.h"

#include <map>

namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// NullChecker
// -------------------------------------------------------------------------- //
  /**
   * Checker that accepts everything.
   */
  struct NullChecker {
    template<class T>
    bool operator()(const T &) const {
      return true;
    }
  };


// -------------------------------------------------------------------------- //
// RangeChecker
// -------------------------------------------------------------------------- //
  /**
   * Checker for integer types.
   *
   * Checks that the value fits into [min, max] interval.
   */
  template<class Integer, Integer min, Integer max, class LessComparator = std::less<Integer> >
  class RangeChecker {
  public:
    RangeChecker() {}

    RangeChecker(const LessComparator &lessComparator): 
      mLessComparator(lessComparator) {}

    bool operator()(const Integer &value) const {
      return !mLessComparator(value, min) && !mLessComparator(max, value);
    }

  private:
    LessComparator mLessComparator;
  };

}} // namespace arx::xml

#endif // ARX_XML_CHECKERS_H
