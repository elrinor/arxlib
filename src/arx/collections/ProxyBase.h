#ifndef __ARX_PROXYBASE_H__
#define __ARX_PROXYBASE_H__

#include "config.h"
#include "ProxyAccess.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ProxyBase
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class ProxyBase {
  public:
    typedef Derived derived_type;
    typedef Container container_type;

  protected:    
    /* 
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

    container_type& container() {
      return ProxyAccess().container<container_type>(derived());
    }

    const container_type& container() const {
      return ProxyAccess().container<container_type>(derived());
    }

    derived_type& derived() {
      return static_cast<derived_type&>(*this);
    }

    const derived_type& derived() const {
      return static_cast<const derived_type&>(*this);
    }
  };

} // namespace arx

#endif // __ARX_PROXYBASE_H__
