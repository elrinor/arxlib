#ifndef ARX_QT_XML_DATA_BINDING_H
#define ARX_QT_XML_DATA_BINDING_H

#include "config.h"
#include <typeinfo>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

class QString;
class QDomNode;

namespace arx {
  class XmlDataBindingContext;

  namespace detail {
    class XmlDataBindingPrivate;
  } // namespace detail

// -------------------------------------------------------------------------- //
// XmlDataBinding
// -------------------------------------------------------------------------- //
  class XmlDataBinding: public boost::noncopyable {
  public:
    typedef boost::function<void (const XmlDataBindingContext &, QDomNode &, void *)> Setter;
    typedef boost::function<void (const XmlDataBindingContext &, const void *, QDomNode *)> Getter;
    typedef boost::function<void (const XmlDataBindingContext &, const void *)> Checker;
    typedef boost::function<bool (const void *)> Condition;

    XmlDataBinding(const std::type_info &typeInfo, const Checker &checker = Checker());

    const std::type_info &typeInfo() const;

    XmlDataBinding &operator() (const QString &path, const Getter &getter, const Setter &setter);

    XmlDataBinding &if_(const Condition &cond);
    XmlDataBinding &else_();
    XmlDataBinding &fi_();

  private:
    const std::type_info &mTypeInfo;
    const Checker mChecker;

    //boost::intrusive_ptr<detail::XmlDataBindingPrivate> mImpl;
  };


// -------------------------------------------------------------------------- //
// TypedXmlDataBinding
// -------------------------------------------------------------------------- //
  template<class Bound>
  class TypedXmlDataBinding: public XmlDataBinding {
  public:
    using XmlDataBinding::operator();

    template<class T, class Binded, class Checker>
    XmlDataBinding &operator() (const QString &path, T Binded::*field) {
      assert(typeid(Binded) == typeInfo());

      return operator()(
        path,
        [](const XmlDataBindingContext &ctx, void *value, QDomNode *node) -> bool {
          T result;
          /* deserialize result. */
          static_cast<Binded *>(value)->*field = result;
      },
        [](const XmlDataBindingContext &ctx, const void *value, QDomNode *node) -> bool {
          /* serialize. */

          return true;
      }
      );
    }

  };

} // namespace arx 

#endif // ARX_QT_XML_DATA_BINDING_H
