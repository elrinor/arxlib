/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
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
#ifndef ARX_EXT_QT_XML_MAPPINGS_H
#define ARX_EXT_QT_XML_MAPPINGS_H

#include <arx/config.h>
#include <boost/range/value_type.hpp>
#include <QApplication> /* For QCoreApplication::translate. */
#include <arx/range/Insert.h>
#include <arx/Foreach.h>
#include "XmlMapping.h"


namespace arx { namespace xml {
// -------------------------------------------------------------------------- //
// QString mapping
// -------------------------------------------------------------------------- //
  template<class Mapper>
  void xml_map_QString(Mapper &mapper, QString &value) {
    mapper.text(value);
  }

  ARX_XML_DEFINE_MAPPING_FUNCTIONS(xml_map_QString, QString)


// -------------------------------------------------------------------------- //
// Integer types mapping
// -------------------------------------------------------------------------- //
#define ARX_XML_DEFINE_SERIALIZATION_FUNC(TYPE, EXPR)                           \
  inline void xml_serialize(Serializer &serializer, const TYPE &x) {            \
    serializer.text(EXPR);                                                      \
  }

  ARX_XML_DEFINE_SERIALIZATION_FUNC(bool,                         QString::fromLatin1(x ? "1" : "0"));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(char,                         QString::number(static_cast<int>(x)));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(unsigned char,                QString::number(static_cast<unsigned int>(x)));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(short,                        QString::number(static_cast<int>(x)));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(unsigned short,               QString::number(static_cast<unsigned int>(x)));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(int,                          QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(unsigned int,                 QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(long,                         QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(unsigned long,                QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(long long,                    QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(unsigned long long,           QString::number(x));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(float,                        QString::number(static_cast<float>(x), 'g', 9));
  ARX_XML_DEFINE_SERIALIZATION_FUNC(double,                       QString::number(x, 'g', 18));
#undef ARX_XML_DEFINE_SERIALIZATION_FUNC

#define ARX_XML_DEFINE_DESERIALIZATION_FUNC(TYPE, INTERMEDIATE_EXPR, CHECK_EXPR) \
  inline void xml_deserialize(Deserializer &deserializer, TYPE &v) {            \
    QString s = deserializer.text();                                            \
    bool ok = true;                                                             \
    decltype(INTERMEDIATE_EXPR) x = (INTERMEDIATE_EXPR);                        \
    if(ok)                                                                      \
      ok = (CHECK_EXPR);                                                        \
    if(ok) {                                                                    \
      v = static_cast<TYPE>(x);                                                 \
    } else {                                                                    \
      deserializer.handleError(qApp->translate("xml_deserialize", "Cannot deserialize string \"%1\" as %2").arg(s).arg(typeid(TYPE).name())); \
    }                                                                           \
  }

  ARX_XML_DEFINE_DESERIALIZATION_FUNC(
    bool,                       
    (s.size() == 1) ? (s[0] != QChar::fromLatin1('0')) : (ok = false), 
    s[0] == QChar::fromLatin1('0') || s[0] == QChar::fromLatin1('1')
  );

#define ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC(TYPE, INTERMEDIATE_EXPR)        \
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(                                          \
    TYPE,                                                                       \
    INTERMEDIATE_EXPR,                                                          \
    (x >= std::numeric_limits<TYPE>::min() && x <= std::numeric_limits<TYPE>::max()) \
  )

  ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC(char,                   s.toInt(&ok));
  ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC(unsigned char,          s.toInt(&ok));
  ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC(short,                  s.toInt(&ok));
  ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC(unsigned short,         s.toInt(&ok));
#undef ARX_XML_DEFINE_INT_DESERIALIZATION_FUNC

  ARX_XML_DEFINE_DESERIALIZATION_FUNC(int,                        s.toInt(&ok),        true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(unsigned int,               s.toUInt(&ok),       true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(long,                       s.toLong(&ok),       true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(unsigned long,              s.toULong(&ok),      true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(long long,                  s.toLongLong(&ok),   true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(unsigned long long,         s.toULongLong(&ok),  true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(float,                      s.toFloat(&ok),      true);
  ARX_XML_DEFINE_DESERIALIZATION_FUNC(double,                     s.toDouble(&ok),     true);
#undef ARX_XML_DEFINE_DESERIALIZATION_FUNC


// -------------------------------------------------------------------------- //
// Collection types mapping
// -------------------------------------------------------------------------- //
  template<class Collection>
  QString xml_collection_element_name(Collection *) {
    return QString("elem");
  }
  
  template<class Collection>
  void xml_serialize(Serializer &serializer, const Collection &x) {
    typedef typename boost::range_value<Collection>::type value_type;

    QString elementName = xml_collection_element_name(static_cast<Collection *>(NULL));
    foreach(const value_type &value, x) {
      serializer.element(elementName, [&] {
        arx::xml::serialize(serializer, value);
      });
    }
  }

  template<class Collection>
  void xml_deserialize(Deserializer &deserializer, Collection &x) {
    typedef typename boost::range_value<Collection>::type value_type;

    QString elementName = xml_collection_element_name(static_cast<Collection *>(NULL));
    while(deserializer.hasElement()) {
      deserializer.element(elementName, [&] {
        value_type value;
        if(arx::xml::deserialize(deserializer, value))
          arx::insert(x, boost::end(x), std::move(value));
      });
    }
  }


}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_BINDINGS_H
