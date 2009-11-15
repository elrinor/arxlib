#ifndef __ARX_MAPPROXYMIXIN_H__
#define __ARX_MAPPROXYMIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// MapProxyMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapProxyMixin: public Base {
  public:
    typedef typename Base::container_type::mapped_type mapped_type;

    mapped_type& operator[] (const typename Base::key_type& key) {
      return container().operator[] (key);
    }
  };

} // namespace arx

#endif // __ARX_MAPPROXYMIXIN_H__
