#ifndef __ARX_ORDERED_SET_FACADE_H__
#define __ARX_ORDERED_SET_FACADE_H__

#include "config.h"
#include "SetFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// OrderedSetFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class OrderedSetFacade: public SetFacade<Derived, Container> {
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

#endif // __ARX_ORDERED_SET_FACADE_H__
