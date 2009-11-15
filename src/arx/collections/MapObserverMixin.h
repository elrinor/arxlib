#ifndef __ARX_MAPOBSERVERMIXIN_H__
#define __ARX_MAPOBSERVERMIXIN_H__

#include "config.h"
#include <cassert>
#include "SetObserverMixin.h"

namespace arx {
// -------------------------------------------------------------------------- //
// MapObserverMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapObserverMixin: public Base {
  public:
    using Base::operator [];

    const typename Base::mapped_type& operator[] (const typename Base::key_type& key) const {
      assert(derived().contains(key));

      return derived.find(key)->second;
    }
  };

} // namespace arx

#endif // __ARX_MAPOBSERVERMIXIN_H__
