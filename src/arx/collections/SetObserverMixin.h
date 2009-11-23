#ifndef __ARX_COLLECTIONS_SET_OBSERVER_MIXIN_H__
#define __ARX_COLLECTIONS_SET_OBSERVER_MIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// SetObserverMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class SetObserverMixin: public Base {
    ARX_INHERIT_FACADE_BASE(Base);
  public:
    bool contains(const typename Base::key_type& key) {
      return derived().find(key) != derived().end();
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_SET_OBSERVER_MIXIN_H__
