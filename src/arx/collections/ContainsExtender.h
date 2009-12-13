#ifndef __ARX_COLLECTIONS_CONTAINS_EXTENDER_H__
#define __ARX_COLLECTIONS_CONTAINS_EXTENDER_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "ExtenderBase.h"
#include "Forward.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ContainsExtender
// -------------------------------------------------------------------------- //
  template<class Base>
  class ContainsExtender: public Base {
  public:
    ARX_FORWARD_CONSTRUCTOR(ContainsExtender, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

    bool contains(const typename Base::key_type& key) const {
      return find(key) != end();
    }
  };

} // namespace arx

ARX_REGISTER_EXTENDER(ContainsExtender, boost::mpl::and_<has_key_type<T>, has_find<T> >);

#endif // __ARX_COLLECTIONS_CONTAINS_EXTENDER_H__
