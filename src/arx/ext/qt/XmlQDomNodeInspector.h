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
#ifndef ARX_EXT_QT_XML_Q_DOM_NODE_INSPECTOR_H
#define ARX_EXT_QT_XML_Q_DOM_NODE_INSPECTOR_H

#include "config.h"
#include <cassert>
#include <QString>
#include <QDomNode>
#include "XmlNodeInspector.h"

namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// XmlQDomNodeInspector
// -------------------------------------------------------------------------- //
  class XmlQDomNodeInspector {
  public:
    QString name(const QDomNode &node) const {
      assert(node.isAttr() || node.isElement());

      return node.nodeName();
    }

    QString value(const QDomNode &node) const {
      assert(node.isAttr() || node.isElement());

      if(node.isAttr())
        return node.toAttr().value();
      else
        return node.toElement().text();
    }
  
    ErrorLocation location(const QDomNode &node) const {
      return ErrorLocation(node.lineNumber(), node.columnNumber());
    }

  };

  template<>
  struct node_inspector<QDomNode> {
    typedef XmlQDomNodeInspector type;
  };

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_Q_DOM_NODE_INSPECTOR_H
