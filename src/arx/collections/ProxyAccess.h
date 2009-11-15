#ifndef __ARX_PROXYACCESS_H__
#define __ARX_PROXYACCESS_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ProxyAccess
// -------------------------------------------------------------------------- //
  class ProxyAccess {
  public:
    template<class Container, class Derived>
    Container& container(Derived& derived) const {
      return derived.crtpContainer();
    }

    template<class Container, class Derived>
    const Container& container(const Derived& derived) const {
      return derived.crtpContainer();
    }
  };

} // namespace arx

#endif // __ARX_PROXYACCESS_H__
