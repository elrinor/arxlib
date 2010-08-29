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
#ifndef ARX_XML_ERROR_H
#define ARX_XML_ERROR_H

#include "config.h"

namespace arx { namespace xml {
  /**
   * Error severity.
   */
  enum ErrorSeverity {
    ERROR,
    WARNING
  };


  /**
   * Error location.
   */
  class ErrorLocation {
  public:
    ErrorLocation(int line, int column): mLine(line), mColumn(column) {}

    int line() const {
      return mLine;
    }

    int column() const {
      return mColumn;
    }

  private:
    int mLine, mColumn;
  };


  /**
   * Base class for all xml errors.
   */
  struct error {};


  /**
   * Node not found error.
   */
  template<class String>
  struct node_not_found: error {
    node_not_found(const String &path):
      path(path) {}

    const String &path;
  };

  template<class String>
  node_not_found<String> create_node_not_found(const String &path) {
    return node_not_found<String>(path);
  }


  /**
   * Invalid value error.
   */
  template<class String>
  struct invalid_value: error {
    invalid_value(const String &value): 
      value(value) {}

    const String &value;
  };

  template<class String>
  invalid_value<String> create_invalid_value(const String &value) {
    return invalid_value<String>(value)
  }


  /**
   * Typed invalid value error.
   */
  template<class T, class String>
  struct invalid_value_for_type: invalid_value<String> {
    invalid_value_for_type(const String &value):
      invalid_value(value) {};

    typedef T type;
  };

  template<class T, class String>
  invalid_value_for_type<T, String> create_invalid_value_for_type(const String &value) {
    return invalid_value_for_type<T, String>(value);
  }

}} // namespace arx::xml

#endif // ARX_XML_ERROR_H
