#ifndef ARX_EXT_QT_XML_DATA_BINDER_H
#define ARX_EXT_QT_XML_DATA_BINDER_H

#include "config.h"
#include <boost/function.hpp>
#include <arx/StaticBlock.h>
#include "XmlDataBinding.h"

namespace arx {
  namespace detail {
    class XmlDataBinderDeleter;
  }

  /**
   * Extension point of XmlDataBinder.
   */
  template<class T>
  XmlDataBinding newXmlDataBinding(T *) {
    static_assert(false, "Implement this function to support serialization and deserialization of custom types");

    Unreachable();
  }


// -------------------------------------------------------------------------- //
// XmlDataBinder
// -------------------------------------------------------------------------- //
  class XmlDataBinder {
  public:
    static XmlDataBinder *instance();

    template<class T>
    void serialize(const T &value, QDomNode *target) const {
      serialize(binding<T>(), value, target);
    }

    template<class T>
    void deserialize(QDomNode &source, T *value) const {
      deserialize(binding<T>(), source, value);
    }
    
  protected:
    XmlDataBinder() {}
    ~XmlDataBinder() {}

    typedef boost::function<XmlDataBinding()> BindingFactory;

    template<class T>
    XmlDataBinding binding() {
      return binding(
        typeid(T), 
        []{ return newXmlDataBinding(static_cast<T *>(NULL)); }
      );
    }

    XmlDataBinding binding(const std::type_info &typeInfo, const BindingFactory &factory);

  private:
    friend class XmlDataBinderDeleter;

    static void destroyInstance();

    class Impl;
    static Impl *sInstance;
  };

  namespace detail {
    ARX_NAMED_STATIC_DEINITIALIZER(XmlDataBinderDeleter) { XmlDataBinder::destroyInstance(); }
  }

} // namespace arx 

#endif // ARX_EXT_QT_XML_DATA_BINDER_H