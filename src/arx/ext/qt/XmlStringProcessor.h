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
#ifndef ARX_EXT_QT_XML_STRING_PROCESSOR_H
#define ARX_EXT_QT_XML_STRING_PROCESSOR_H

#include "config.h"

namespace arx { namespace xml {
  namespace xml_string_processor_detail {
// -------------------------------------------------------------------------- //
// Wrappers
// -------------------------------------------------------------------------- //
    template<class PathElement>
    auto to_string_wrapper(const PathElement &pathElement) -> 
      decltype(to_string(pathElement)) 
    {
      return to_string(pathElement);
    }

    template<class String>
    String append_element_wrapper(const String &path, const String &elementName) {
      return append_element(path, elementName);
    }

    template<class String>
    String append_attribute_wrapper(const String &path, const String &attributeName) {
      return append_attribute(path, attributeName);
    }
  
  } // namespace xml_string_processor_detail


// -------------------------------------------------------------------------- //
// DefaultStringProcessor
// -------------------------------------------------------------------------- //
  class DefaultStringProcessor {
  public:
    template<class PathElement>
    auto to_string(const PathElement &pathElement) const -> 
      decltype(xml_string_processor_detail::to_string_wrapper(pathElement)) 
    {
      return xml_string_processor_detail::to_string_wrapper(pathElement);
    }

    template<class String>
    String append_element(const String &path, const String &elementName) const {
      return xml_string_processor_detail::append_element_wrapper(path, elementName);
    }

    template<class String>
    String append_attribute(const String &path, const String &attributeName) const {
      return xml_string_processor_detail::append_attribute_wrapper(path, attributeName);
    }
  };


// -------------------------------------------------------------------------- //
// string_processor
// -------------------------------------------------------------------------- //
  /** 
   * Specialize for your node type, or define global functions that can be 
   * found via ADL.
   */
  template<class Node>
  struct node_string_processor {
    typedef DefaultStringProcessor type;
  };

  template<class String>
  struct string_processor {
    typedef DefaultStringProcessor type;
  };

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_STRING_PROCESSOR_H
