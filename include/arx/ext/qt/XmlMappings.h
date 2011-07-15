#ifndef ARX_EXT_QT_XML_MAPPINGS_H
#define ARX_EXT_QT_XML_MAPPINGS_H

#include <arx/config.h>
#include <QApplication>
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
// List types mapping
// -------------------------------------------------------------------------- //



}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_BINDINGS_H
