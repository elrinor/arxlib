#ifndef __ARX_COLLECTIONS_RA_SEQUENCE_FACADE_H__
#define __ARX_COLLECTIONS_RA_SEQUENCE_FACADE_H__

#include "config.h"
#include "SequenceFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// RaSequenceFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class RaSequenceFacade: public SequenceFacade<Derived, Container> {
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

#endif // __ARX_COLLECTIONS_RA_SEQUENCE_FACADE_H__
