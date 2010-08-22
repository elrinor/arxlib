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
#ifndef ARX_NULL_H
#define ARX_NULL_H

#include <boost/noncopyable.hpp>
#include <boost/utility.hpp>            /* for boost::addressof */

namespace arx {
  namespace null_adl_protected {
    class null_t: public boost::noncopyable {
      bool operator== (const null_t&) {
        return true;
      }

      bool operator!= (const null_t&) {
        return false;
      }

      bool operator== (int value) {
        return value == 0;
      }

      bool operator!= (int value) {
        return value != 0;
      }
    };
  } // namespace null_adl_protected

  typedef null_adl_protected::null_t null_t;

  /* Declaring it static is OK and it solves the problem with multiple 
   * definitions when linking. */
  static null_t static_null;

} // namespace arx

/**
 * Typed null value, for use in expressions only.
 */
#define null (::arx::static_null)

/**
 * Null-comparable interface. For use in class definitions.
 *
 * @param CLASS class name.
 * @param NULL_CHECK_EXPR expression that checks whether this object is null.
 */
#define ARX_NULL_COMPARABLE(CLASS, NULL_CHECK_EXPR)                             \
  bool operator== (const arx::null_t&) const {                                  \
    return NULL_CHECK_EXPR;                                                     \
  }                                                                             \
                                                                                \
  bool operator!= (const arx::null_t& r) const {                                \
    return !operator== (r);                                                     \
  }                                                                             \
                                                                                \
  friend bool operator== (const arx::null_t& l, const CLASS& r) {               \
    return r == l;                                                              \
  }                                                                             \
                                                                                \
  friend bool operator!= (const arx::null_t& l, const CLASS& r) {               \
    return !(l == r);                                                           \
  }                                                                             \

//#define ARX_IS_NULL(NULL_CHECK_EXPR)

/* TODO: think NULL_ASSIGNABLE / NULL_CONSTRUCTIBLE */

#endif // ARX_NULL_H
