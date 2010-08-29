#ifndef ARX_EXT_QT_STANDARD_XML_BINDINGS_H
#define ARX_EXT_QT_STANDARD_XML_BINDINGS_H

#include "config.h"
#include <cassert>
#include <limits>
#include "XmlBinding.h"
#include "XmlErrorData.h"

namespace arx { namespace xml {
  namespace standard_xml_bindings_detail {
    /**
     * Serialization function for QStrings.
     */
    inline void serialize(const QString &source, QDomNode *target) {
      assert((target->isAttr() && target->toAttr().value().isNull()) || (target->isElement() && target->childNodes().size() == 0));

      if(target->isAttr())
        target->toAttr().setValue(source);
      else
        target->appendChild(target->ownerDocument().createTextNode(source));
    }


    /**
     * Deserialization function for QStrings.
     */
    inline QString deserialize(const QDomNode &source) {
      assert(source.isAttr() || source.isElement());

      if(source.isAttr())
        return source.toAttr().value();
      else
        return source.toElement().text();
    }


    /**
     * Serializer functor that implements serialization of basic C++ types and 
     * QStrings.
     */
    struct Serializer {
      template<class MessageHandler, class Params>
      void operator()(const QString &source, MessageHandler &, const Params &, QDomNode *target) const {
        serialize(source, target);
      }

#define ARX_SERIALIZATION_FUNC(TYPE, EXPR)                                      \
      template<class MessageHandler, class Params>                              \
      void operator()(TYPE x, MessageHandler &, const Params &, QDomNode *target) const { \
        serialize(EXPR, target);                                                \
      }

      ARX_SERIALIZATION_FUNC(bool,                         QString::fromLatin1(x ? "1" : "0"));
      ARX_SERIALIZATION_FUNC(char,                         QString::number(static_cast<int>(x)));
      ARX_SERIALIZATION_FUNC(unsigned char,                QString::number(static_cast<unsigned int>(x)));
      ARX_SERIALIZATION_FUNC(short,                        QString::number(static_cast<int>(x)));
      ARX_SERIALIZATION_FUNC(unsigned short,               QString::number(static_cast<unsigned int>(x)));
      ARX_SERIALIZATION_FUNC(int,                          QString::number(x));
      ARX_SERIALIZATION_FUNC(unsigned int,                 QString::number(x));
      ARX_SERIALIZATION_FUNC(long,                         QString::number(x));
      ARX_SERIALIZATION_FUNC(unsigned long,                QString::number(x));
      ARX_SERIALIZATION_FUNC(long long,                    QString::number(x));
      ARX_SERIALIZATION_FUNC(unsigned long long,           QString::number(x));
      ARX_SERIALIZATION_FUNC(float,                        QString::number(static_cast<float>(x), 'g', 9));
      ARX_SERIALIZATION_FUNC(double,                       QString::number(x, 'g', 18));
#undef ARX_SERIALIZATION_FUNC
    };


    /**
     * Deserialization functor that implements deserialization of basic C++
     * types and QStrings.
     */
    struct Deserializer {
      template<class MessageHandler, class Params>
      void operator()(QDomNode &source, MessageHandler &, const Params &, QString *target) const {
        *target = deserialize(source);
      }

#define ARX_DESERIALIZATION_FUNC(TYPE, INTERMEDIATE_EXPR, CHECK_EXPR)           \
      template<class MessageHandler, class Params>                              \
      void operator()(QDomNode &source, MessageHandler &handler, const Params &, TYPE *target) const { \
        QString s = deserialize(source);                                        \
        bool ok = true;                                                         \
        decltype(INTERMEDIATE_EXPR) x = (INTERMEDIATE_EXPR);                    \
        if(ok)                                                                  \
          ok = (CHECK_EXPR);                                                    \
        if(ok)                                                                  \
          *target = static_cast<TYPE>(x);                                       \
        else                                                                    \
          handler(QtFatalMsg, invalid_value_for_type<TYPE>(s), QSourceLocation(QUrl(), source.lineNumber(), source.columnNumber())); \
      }

      ARX_DESERIALIZATION_FUNC(bool,                       
        (s.size() == 1) ? (s[0] != QChar::fromLatin1('0')) : (ok = false), 
        s[0] == QChar::fromLatin1('0') || s[0] == QChar::fromLatin1('1')
      );

#define ARX_INT_DESERIALIZATION_FUNC(TYPE, INTERMEDIATE_EXPR)                   \
      ARX_DESERIALIZATION_FUNC(TYPE, INTERMEDIATE_EXPR, (x >= std::numeric_limits<TYPE>::min() && x <= std::numeric_limits<TYPE>::max()))

      ARX_INT_DESERIALIZATION_FUNC(char,                   s.toInt(&ok));
      ARX_INT_DESERIALIZATION_FUNC(unsigned char,          s.toInt(&ok));
      ARX_INT_DESERIALIZATION_FUNC(short,                  s.toInt(&ok));
      ARX_INT_DESERIALIZATION_FUNC(unsigned short,         s.toInt(&ok));
#undef ARX_INT_DESERIALIZATION_FUNC

      ARX_DESERIALIZATION_FUNC(int,                        s.toInt(&ok),        true);
      ARX_DESERIALIZATION_FUNC(unsigned int,               s.toUInt(&ok),       true);
      ARX_DESERIALIZATION_FUNC(long,                       s.toLong(&ok),       true);
      ARX_DESERIALIZATION_FUNC(unsigned long,              s.toULong(&ok),      true);
      ARX_DESERIALIZATION_FUNC(long long,                  s.toLongLong(&ok),   true);
      ARX_DESERIALIZATION_FUNC(unsigned long long,         s.toULongLong(&ok),  true);
      ARX_DESERIALIZATION_FUNC(float,                      s.toFloat(&ok),      true);
      ARX_DESERIALIZATION_FUNC(double,                     s.toDouble(&ok),     true);
#undef ARX_DESERIALIZATION_FUNC
    };

  } // namespace standard_xml_bindings_detail


#define ARX_STANDARD_BINDING(TYPE)                                              \
  ARX_DEFINE_XML_BINDING(                                                       \
    TYPE,                                                                       \
    functional(                                                                 \
      self,                                                                     \
      standard_xml_bindings_detail::Serializer(),                               \
      standard_xml_bindings_detail::Deserializer()                              \
    )                                                                           \
  );

  ARX_STANDARD_BINDING(QString);
  ARX_STANDARD_BINDING(bool);
  ARX_STANDARD_BINDING(char);
  ARX_STANDARD_BINDING(unsigned char);
  ARX_STANDARD_BINDING(short);
  ARX_STANDARD_BINDING(unsigned short);
  ARX_STANDARD_BINDING(int);
  ARX_STANDARD_BINDING(unsigned int);
  ARX_STANDARD_BINDING(long);
  ARX_STANDARD_BINDING(unsigned long);
  ARX_STANDARD_BINDING(long long);
  ARX_STANDARD_BINDING(unsigned long long);
  ARX_STANDARD_BINDING(float);
  ARX_STANDARD_BINDING(double);
#undef ARX_STANDARD_BINDING

}} // namespace arx::xml

#endif // ARX_EXT_QT_STANDARD_XML_BINDINGS_H
