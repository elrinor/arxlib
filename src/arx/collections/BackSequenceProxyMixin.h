#ifndef __ARX_BACKSEQUENCEPROXYMIXIN_H__
#define __ARX_BACKSEQUENCEPROXYMIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// BackSequenceProxyMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class BackSequenceProxyMixin: public Base {
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

#endif // __ARX_BACKSEQUENCEPROXYMIXIN_H__