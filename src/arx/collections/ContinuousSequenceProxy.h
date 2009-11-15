#ifndef __ARX_CONTINUOUSSEQUENCEPROXY_H__
#define __ARX_CONTINUOUSSEQUENCEPROXY_H__

#include "config.h"
#include "RaSequenceProxy.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ContinuousSequenceProxy
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class ContinuousSequenceProxy: public RaSequenceProxy<Derived, Container> {
  public:
    void reserve(size_type count) {
      container().reserve(count);
    }
  };

} // namespace arx

#endif // __ARX_CONTINUOUSSEQUENCEPROXY_H__
