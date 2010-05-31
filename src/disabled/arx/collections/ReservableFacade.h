#ifndef __ARX_COLLECTIONS_RESERVABLE_FACADE_H__
#define __ARX_COLLECTIONS_RESERVABLE_FACADE_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// ReservableFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class ReservableFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (size_type));

    void reserve(size_type count) {
      this->container().reserve(count);
    }

    size_type capacity() const {
      return this->container().capacity();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(ReservableFacade, boost::mpl::and_<has_reserve<T>, has_capacity<T> >)

} // namespace arx

#endif // __ARX_COLLECTIONS_RESERVABLE_FACADE_H__
