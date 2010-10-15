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
#ifndef ARX_EXT_QT_DOM_XML_RECEIVER_H
#define ARX_EXT_QT_DOM_XML_RECEIVER_H

#include "config.h"
#include <QAbstractXmlReceiver>
#include <QDomNode>
#include <QXmlQuery>
#include <QList>

namespace arx {
// -------------------------------------------------------------------------- //
// DomXmlReceiver
// -------------------------------------------------------------------------- //
  class DomXmlReceiver: public QAbstractXmlReceiver {
  public:
    DomXmlReceiver(const QXmlQuery &query, QDomNode *outNode);

    virtual void atomicValue(const QVariant &value);

    virtual void attribute(const QXmlName &name, const QStringRef &value);

    virtual void characters(const QStringRef &value);

    virtual void comment(const QString &value);

    virtual void endDocument();

    virtual void endElement();

    virtual void endOfSequence();

    virtual void namespaceBinding(const QXmlName &name);

    virtual void processingInstruction(const QXmlName &target, const QString &value);

    virtual void startDocument();

    virtual void startElement(const QXmlName &name);

    virtual void startOfSequence();

  protected:
    QDomNode appendChild(QDomNode &node);

    QDomDocument document();

  private:
    const QXmlQuery mQuery;
    QList<QDomNode> mNodeStack;
  };

} // namespace arx

#endif // ARX_EXT_QT_DOM_XML_RECEIVER_H
