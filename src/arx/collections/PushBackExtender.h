#ifndef __ARX_COLLECTIONS_PUSH_BACK_EXTENDER_H__
#define __ARX_COLLECTIONS_PUSH_BACK_EXTENDER_H__

#include "config.h"
#include "ExtenderBase.h"
#include "Forward.h"

namespace arx {
// -------------------------------------------------------------------------- //
// PushBackExtender
// -------------------------------------------------------------------------- //
  template<class Base>
  class PushBackExtender: public Base {
  public:
    ARX_FORWARD_CONSTRUCTOR(PushBackExtender, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

    using Base::push_back;

    void push_back() {
      push_back(typename Base::value_type());
    }
  };

} // namespace arx

ARX_REGISTER_EXTENDER(PushBackExtender, has_push_back<T>)

#endif // __ARX_COLLECTIONS_PUSH_BACK_EXTENDER_H__
