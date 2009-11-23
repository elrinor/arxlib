#ifndef __ARX_COLLECTIONS_BACK_SEQUENCE_FACADE_MIXIN_H__
#define __ARX_COLLECTIONS_BACK_SEQUENCE_FACADE_MIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// BackSequenceFacadeMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class BackSequenceFacadeMixin: public Base {
    ARX_INHERIT_FACADE_BASE(Base);
  public:
    void pop_back() {
      container().pop_back();
    }

    void push_back(const typename Base::value_type& value) { /* Differs from stl - reference used. */
      container().push_back(value);
    }

    /* Additional mutators. */

    void push_back() {
      push_back(typename Base::value_type());
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_BACK_SEQUENCE_FACADE_MIXIN_H__
