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
#ifndef ARX_EXT_QT_DEFAULT_NODE_WALKER_H
#define ARX_EXT_QT_DEFAULT_NODE_WALKER_H

#include "config.h"

namespace arx { namespace xml {
  
  template<class Node>
  bool is_null_wrapper(const Node &node) {
    return is_null(node);
  }

  template<class Node, class String>
  Node attribute_wrapper(const Node &node, const String &name) {
    return attribute(node, name);
  }

  template<class Node, class String>
  Node element_wrapper(const Node &node, const String &name) {
    return element(node, name);
  }

  template<class Node, class String>
  Node create_attribute_wrapper(Node &node, const String &name) {
    return create_attribute(node, name);
  }

  template<class Node, class String>
  Node create_element_wrapper(Node &node, const String &name) {
    return create_element(node, name);
  }

// -------------------------------------------------------------------------- //
// DefaultNodeWalker
// -------------------------------------------------------------------------- //
  class DefaultNodeWalker {
  public:
    template<class Node>
    bool is_null(const Node &node) const {
      return is_null_wrapper(node);
    }

    template<class Node, class String>
    Node attribute(const Node &node, const String &name) const {
      return attribute_wrapper(node, name);
    }

    template<class Node, class String>
    Node element(const Node &node, const String &name) const {
      return element_wrapper(node, name);
    }

    template<class Node, class String>
    Node create_attribute(Node &node, const String &name) const {
      return create_attribute_wrapper(node, name);
    }

    template<class Node, class String>
    Node create_element(Node &node, const String &name) const {
      return create_element_wrapper(node, name);
    }

  };

}} // namespace arx::xml

#endif // ARX_EXT_QT_DEFAULT_NODE_WALKER_H
