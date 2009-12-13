#ifndef __ARX_COLLECTIONS_PUSH_FRONT_EXTENDER_H__
#define __ARX_COLLECTIONS_PUSH_FRONT_EXTENDER_H__

#include "config.h"
#include "ExtenderBase.h"
#include "Forward.h"

namespace arx {
// -------------------------------------------------------------------------- //
// PushFrontExtender
// -------------------------------------------------------------------------- //
  template<class Base>
  class PushFrontExtender: public Base {
  public:
    ARX_FORWARD_CONSTRUCTOR(PushFrontExtender, (ARX_FWD_TEXT(Base))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

    using Base::push_front;

    void push_front() {
      push_front(typename Base::value_type());
    }
  };

} // namespace arx

ARX_REGISTER_EXTENDER(PushFrontExtender, has_push_front<T>)

#endif // __ARX_COLLECTIONS_PUSH_FRONT_EXTENDER_H__
