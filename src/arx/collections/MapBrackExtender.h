#ifndef __ARX_COLLECTIONS_MAP_BRACK_EXTENDER_H__
#define __ARX_COLLECTIONS_MAP_BRACK_EXTENDER_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "ExtenderBase.h"
#include "Forward.h"

namespace arx {
// -------------------------------------------------------------------------- //
// MapBrackExtender
// -------------------------------------------------------------------------- //
  template<class Base>
  class MapBrackExtender: public Base {
  public:
    ARX_FORWARD_CONSTRUCTOR(MapBrackExtender, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

    using Base::operator [];

    const typename Base::mapped_type& operator[] (const typename Base::key_type& key) const {
      assert(find(key) != Base::end());

      return find(key)->second;
    }
  };

} // namespace arx

ARX_REGISTER_EXTENDER(MapBrackExtender, boost::mpl::and_<has_mapped_type<T>, has_key_type<T>, has_find<T> >)

#endif // __ARX_COLLECTIONS_MAP_BRACK_EXTENDER_H__
