#ifndef __ARX_COLLECTIONS_CONTINUOUS_SEQUENCE_FACADE_H__
#define __ARX_COLLECTIONS_CONTINUOUS_SEQUENCE_FACADE_H__

#include "config.h"
#include "RaSequenceFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ContinuousSequenceFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class ContinuousSequenceFacade: public RaSequenceFacade<Derived, Container> {
  public:
    void reserve(size_type count) {
      container().reserve(count);
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_CONTINUOUS_SEQUENCE_FACADE_H__
