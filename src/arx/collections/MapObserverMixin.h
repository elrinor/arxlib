#ifndef __ARX_COLLECTIONS_MAP_OBSERVER_MIXIN_H__
#define __ARX_COLLECTIONS_MAP_OBSERVER_MIXIN_H__

#include "config.h"
#include <cassert>
#include "SetObserverMixin.h"

namespace arx {
// -------------------------------------------------------------------------- //
// MapObserverMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapObserverMixin: public Base {
    ARX_INHERIT_FACADE_BASE(Base);
  public:
    using Base::operator [];

    const typename Base::mapped_type& operator[] (const typename Base::key_type& key) const {
      assert(derived().contains(key));

      return derived.find(key)->second;
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_MAP_OBSERVER_MIXIN_H__
