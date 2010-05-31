#ifndef __ARX_COLLECTIONS_ALLOCATOR_AWARE_FACADE_H__
#define __ARX_COLLECTIONS_ALLOCATOR_AWARE_FACADE_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "FacadeBase.h"

namespace arx {
// -------------------------------------------------------------------------- //
// AllocatorAwareFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class AllocatorAwareFacade: public Base {
  public:
    typedef typename Base::container_type::allocator_type allocator_type;

    allocator_type get_allocator() const {
      return this->container().get_allocator();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(AllocatorAwareFacade, boost::mpl::and_<has_allocator_type<T>, has_get_allocator<T> >)

} // namespace arx

#endif // __ARX_COLLECTIONS_ALLOCATOR_AWARE_FACADE_H__
