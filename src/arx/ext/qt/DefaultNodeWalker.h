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
