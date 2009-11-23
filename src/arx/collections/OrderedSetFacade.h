#ifndef __ARX_COLLECTIONS_ORDERED_SET_FACADE_H__
#define __ARX_COLLECTIONS_ORDERED_SET_FACADE_H__

#include "config.h"
#include "SetFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// OrderedSetFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class OrderedSetFacade: public SetFacade<Derived, Container> {
    ARX_INHERIT_FACADE_BASE(SetFacade<Derived, Container>);
  public:
    ARX_INJECT_TYPES(container_type, (key_compare)(value_compare));

    key_compare key_comp( ) const {
      return container().key_comp();
    }

    value_compare value_comp( ) const {
      return container().value_comp();
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_ORDERED_SET_FACADE_H__
