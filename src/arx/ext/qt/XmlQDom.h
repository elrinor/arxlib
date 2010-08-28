#ifndef ARX_EXT_QT_XML_Q_DOM_H
#define ARX_EXT_QT_XML_Q_DOM_H

#include "config.h"
#include <cassert>
#include <QString>
#include <QDomNode>
#include <QDomElement>
#include <QDomAttr>

namespace arx { namespace xml {

  namespace detail {
    const QString &to_string(const QString &s) {
      return s;
    }

    QString to_string(const char *s) {
      return QString(s);
    }

    QString to_string(const wchar_t *s) {
      return QString::fromWCharArray(s);
    }

#ifndef QT_NO_STL
    QString to_string(const std::string &s) {
      return QString::fromStdString(s);
    }
#endif // QT_NO_STL

#ifndef QT_NO_STL_WCHAR
    QString to_string(const std::wstring &s) {
      return QString::fromStdWString(s);
    }
#endif // QT_NO_STL_WCHAR

  } // namespace detail

  bool is_null(const QDomNode &node) {
    return node.isNull();
  }

  template<class String>
  QDomNode attribute(const QDomNode &node, const String &name) {
    assert(node.isElement());

    return node.toElement().attributeNode(detail::to_string(name));
  }

  template<class String>
  QDomNode element(const QDomNode &node, const String &name) {
    assert(node.isElement());

    return node.toElement().firstChildElement(detail::to_string(name));
  }

  template<class String>
  QDomNode create_attribute(QDomNode &node, const String &name) {
    assert(node.isElement());
    assert(node.toElement().attributeNode(detail::to_string(name)).isNull());

    QString qName = detail::to_string(name);
    node.toElement().setAttributeNode(node.ownerDocument().createAttribute(qName));
    return node.toElement().attributeNode(qName);
  }

  template<class String>
  QDomNode create_element(QDomNode &node, const String &name) {
    assert(node.isElement());
    assert(node.firstChildElement(detail::to_string(name)).isNull());

    return node.toElement().appendChild(
      node.ownerDocument().createElement(detail::to_string(name))
    );
  }


}} // namespace arx::xml

#endif // ARX_EXT_QT_XML_Q_DOM_H
