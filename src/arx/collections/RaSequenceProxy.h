#ifndef __ARX_RASEQUENCEPROXY_H__
#define __ARX_RASEQUENCEPROXY_H__

#include "config.h"
#include "SequenceProxy.h"

namespace arx {
// -------------------------------------------------------------------------- //
// RaSequenceProxy
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class RaSequenceProxy: public SequenceProxy<Derived, Container> {
  public:
    reference at(size_type pos) {
      return container().at(pos);
    }

    const_reference at(size_type pos) const {
      return container().at(pos);
    }

    reference operator[] (size_type pos) {
      return container().operator[] (pos);
    }

    const_reference operator[] (size_type pos) const {
      return container().operator[] (pos);
    }
  };

} // namespace arx

#endif // __ARX_RASEQUENCEPROXY_H__
