#ifndef ARX_EXT_QT_STANDARD_XML_BINDINGS_H
#define ARX_EXT_QT_STANDARD_XML_BINDINGS_H

#include "config.h"
#include <cassert>
#include "XmlBinding.h"

namespace arx { namespace xml {

  namespace detail {

    inline void serialize(const QString &source, QDomNode *target) {
      assert((target->isAttr() && target->toAttr().value().isNull()) || (target->isElement() && target->childNodes().size() == 0));

      if(target->isAttr())
        target->toAttr().setValue(source);
      else
        target->appendChild(target->ownerDocument().createTextNode(source));
    }

    inline QString deserialize(const QDomNode &source) {
      assert(source.isAttr() || source.isElement());

      if(source.isAttr())
        return source.toAttr().value();
      else
        return source.toElement().text();
    }

    template<class T>
    struct Serializer;

    template<class T>
    struct Deserializer;

    template<>
    struct Serializer<QString> {
      template<class MessageHandler, class Params>
      void operator()(const QString &source, const MessageHandler &, const Params &, QDomNode *target) const {
        serialize(source, target);
      }
    };

    template<>
    struct Deserializer<QString> {
      template<class MessageHandler, class Params>
      void operator()(QDomNode &source, const MessageHandler &, const Params &, QString *target) const {
        *target = deserialize(source);
      }
    };


  }


  ARX_DEFINE_XML_BINDING(QString, functional(self, detail::Serializer<QString>(), detail::Deserializer<QString>()));


}} // namespace arx::xml

#endif // ARX_EXT_QT_STANDARD_XML_BINDINGS_H
