#ifndef __ARX_MAP_FACADE_MIXIN_H__
#define __ARX_MAP_FACADE_MIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// MapFacadeMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapFacadeMixin: public Base {
  public:
    typedef typename Base::container_type::mapped_type mapped_type;

    mapped_type& operator[] (const typename Base::key_type& key) {
      return container().operator[] (key);
    }
  };

} // namespace arx

#endif // __ARX_MAP_FACADE_MIXIN_H__
