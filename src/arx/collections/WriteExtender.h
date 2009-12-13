#ifndef __ARX_COLLECTIONS_WRITE_EXTENDER_H__
#define __ARX_COLLECTIONS_WRITE_EXTENDER_H__

#include "config.h"
#include "ExtenderBase.h"
#include "Forward.h"

namespace arx {
// -------------------------------------------------------------------------- //
// WriteExtender
// -------------------------------------------------------------------------- //
  template<class Base>
  class WriteExtender: public Base {
  public:
    ARX_FORWARD_CONSTRUCTOR(WriteExtender, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

    typename Base::derived_type& operator<< (const typename Base::value_type& value) {
      insert(end(), value);
      return *static_cast<Base::derived_type*>(this);
    }
  };

} // namespace arx

ARX_REGISTER_EXTENDER(WriteExtender, has_insert<T>)

#endif // __ARX_COLLECTIONS_WRITE_EXTENDER_H__
