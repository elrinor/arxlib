#ifndef __ARX_ORDEREDSETPROXY_H__
#define __ARX_ORDEREDSETPROXY_H__

#include "config.h"
#include "SetProxy.h"

namespace arx {
// -------------------------------------------------------------------------- //
// OrderedSetProxy
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class OrderedSetProxy: public SetProxy<Derived, Container> {
  public:
    typedef typename container_type::key_compare key_compare;
    typedef typename container_type::value_compare value_compare;

    key_compare key_comp( ) const {
      return container().key_comp();
    }

    value_compare value_comp( ) const {
      return container().value_comp();
    }
  };

} // namespace arx

#endif // __ARX_ORDEREDSETPROXY_H__
