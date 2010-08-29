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
#ifndef ARX_EXT_QT_XML_Q_STRING_PROCESSOR_H
#define ARX_EXT_QT_XML_Q_STRING_PROCESSOR_H

#include "config.h"
#include <QString>
#include <arx/xml/StringProcessor.h>

class QDomNode;

namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// XmlQStringProcessor
// -------------------------------------------------------------------------- //
  class XmlQStringProcessor {
  public:
    const QString &to_string(const QString &s) const {
      return s;
    }

    QString to_string(const char *s) const {
      return QString::fromLatin1(s);
    }

    QString to_string(const wchar_t *s) const {
      return QString::fromWCharArray(s);
    }

#ifndef QT_NO_STL
    QString to_string(const std::string &s) const {
      return QString::fromStdString(s);
    }
#endif // QT_NO_STL

#ifndef QT_NO_STL_WCHAR
    QString to_string(const std::wstring &s) const {
      return QString::fromStdWString(s);
    }
#endif // QT_NO_STL_WCHAR

    QString append_element(const QString &path, const QString &elementName) const {
      return path + "/" + elementName;
    }

    QString append_attribute(const QString &path, const QString &attributeName) const {
      return path + "/@" + attributeName;
    }

  };

  template<>
  struct string_processor<QString> {
    typedef XmlQStringProcessor type;
  };

  template<>
  struct node_string_processor<QDomNode> {
    typedef XmlQStringProcessor type;
  };

}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_Q_STRING_PROCESSOR_H
