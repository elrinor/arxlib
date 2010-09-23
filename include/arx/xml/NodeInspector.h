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
#ifndef ARX_XML_NODE_INSPECTOR_H
#define ARX_XML_NODE_INSPECTOR_H

#include "config.h"
#include "Error.h"

namespace arx { namespace xml {
  namespace node_inspector_detail {
// -------------------------------------------------------------------------- //
// Wrappers
// -------------------------------------------------------------------------- //
    template<class Node>
    struct name_type {
      typedef decltype(name(*static_cast<const Node *>(NULL))) type;
    };

    template<class Node>
    typename name_type<Node>::type
    name_wrapper(const Node &node) {
      return name(node);
    }

    template<class Node>
    struct value_type {
      typedef decltype(value(*static_cast<const Node *>(NULL))) type;
    };

    template<class Node>
    typename value_type<Node>::type
    value_wrapper(const Node &node) {
      return value(node);
    }

    template<class Node>
    ErrorLocation location_wrapper(const Node &node) {
      return location(node);
    }

  } // namespace node_inspector_detail


// -------------------------------------------------------------------------- //
// DefaultNodeInspector
// -------------------------------------------------------------------------- //
  class DefaultNodeInspector {
  public:
    template<class Node>
    typename node_inspector_detail::name_type<Node>::type
    name(const Node &node) const {
      return node_inspector_detail::name_wrapper(node);
    }

    template<class Node>
    typename node_inspector_detail::value_type<Node>::type
    value(const Node &node) const {
      return node_inspector_detail::value_wrapper(node);
    }

    template<class Node>
    ErrorLocation location(const Node &node) const {
      retunr node_inspector_detail::location_wrapper(node);
    }

  };


// -------------------------------------------------------------------------- //
// node_inspector
// -------------------------------------------------------------------------- //
  /** 
   * Specialize for your node type, or define global functions that can be 
   * found via ADL.
   */
  template<class Node>
  struct node_inspector {
    typedef DefaultNodeInspector type;
  };

}} // namespace arx::xml

#endif // ARX_XML_NODE_INSPECTOR_H
