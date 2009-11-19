#ifndef __ARX_COLLECTIONS_FACADE_BASE_H__
#define __ARX_COLLECTIONS_FACADE_BASE_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// FacadeBase
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class FacadeBase {
  public:
    typedef Derived derived_type;
    typedef Container container_type;

  protected:    
    /* 
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

    container_type& container() {
      return derived().crtpContainer();
    }

    const container_type& container() const {
      return derived().crtpContainer();
    }

    derived_type& derived() {
      return static_cast<derived_type&>(*this);
    }

    const derived_type& derived() const {
      return static_cast<const derived_type&>(*this);
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_FACADE_BASE_H__
