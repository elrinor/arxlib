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
#ifndef ARX_EXT_QT_XML_Q_DOM_NODE_WALKER_H
#define ARX_EXT_QT_XML_Q_DOM_NODE_WALKER_H

#include <arx/config.h>
#include <cassert>
#include <QString>
#include <QDomNode>
#include <arx/xml/NodeWalker.h>

namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// XmlQDomNodeWalker
// -------------------------------------------------------------------------- //
  class XmlQDomNodeWalker {
  public:
    bool is_null(const QDomNode &node) const {
      return node.isNull();
    }

    QDomNode attribute(const QDomNode &node, const QString &name) const {
      assert(node.isElement());

      return node.toElement().attributeNode(name);
    }

    QDomNode element(const QDomNode &node, const QString &name) const {
      assert(node.isElement() || node.isDocument() || node.isDocumentFragment());

      return node.firstChildElement(name);
    }

    QDomNode create_attribute(QDomNode &node, const QString &name) const {
      assert(node.isElement());
      assert(node.toElement().attributeNode(name).isNull());

      node.toElement().setAttributeNode(node.ownerDocument().createAttribute(name));
      return node.toElement().attributeNode(name);
    }

    QDomNode create_element(QDomNode &node, const QString &name) const {
      assert(node.isElement() || node.isDocument() || node.isDocumentFragment());
      assert(node.firstChildElement(name).isNull());

      return node.appendChild(node.ownerDocument().createElement(name));
    }
  
  };

  template<>
  struct node_walker<QDomNode> {
    typedef XmlQDomNodeWalker type;
  };

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_Q_DOM_NODE_WALKER_H
