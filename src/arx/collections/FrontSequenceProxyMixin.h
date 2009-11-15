#ifndef __ARX_FRONTSEQUENCEPROXYMIXIN_H__
#define __ARX_FRONTSEQUENCEPROXYMIXIN_H__

#include "config.h"

namespace arx {
// -------------------------------------------------------------------------- //
// BackSequenceProxyMixin
// -------------------------------------------------------------------------- //
  template<class Base>
  class BackSequenceProxyMixin: public Base {
  public:
    void pop_front() {
      container().pop_front();
    }

    void push_front(const value_type& value) { /* Differs from stl - reference used. */
      container().push_front(value);
    }

    /* Additional mutators. */

    void push_front() {
      push_front(value_type());
    }
  };

} // namespace arx

#endif // __ARX_FRONTSEQUENCEPROXYMIXIN_H__
