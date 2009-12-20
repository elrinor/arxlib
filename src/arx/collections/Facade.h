#ifndef __ARX_COLLECTIONS_FACADE_H__
#define __ARX_COLLECTIONS_FACADE_H__

#include "config.h"
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include "FacadeBase.h"

/* Register facades. */
#include "AllocatorAwareFacade.h"
#include "AssociativeFacade.h"
#include "BackFrontFacades.h"
#include "MapFacade.h"
#include "PushPopableFacades.h"
#include "IndexFacades.h"
#include "RangeFacades.h"
#include "ReservableFacade.h"
#include "SequenceFacade.h"
#include "UnorderedFacade.h"

namespace arx {
  namespace detail {
    namespace facade_base_ns {
      using namespace boost::mpl;
// -------------------------------------------------------------------------- //
// facade_base
// -------------------------------------------------------------------------- //
      template<class Container, class Derived, long n = ARX_GET_COUNTER(facade_count) - 1>
      struct facade_base {
        typedef typename facade_base<Container, Derived, n - 1>::type current_base;

        typedef typename eval_if<typename RegisteredFacade<n>::template applicable_to<Container>::type,
          typename RegisteredFacade<n>::template apply<current_base>,
          identity<current_base>
        >::type type;
      };

      template<class Container, class Derived>
      struct facade_base<Container, Derived, -1> {
        typedef FacadeBase<Container, Derived> type;
      };

    } // namespace facade_base_ns
  } // namespace detail


// -------------------------------------------------------------------------- //
// Facade
// -------------------------------------------------------------------------- //
  template<class Container, class Derived>
  class Facade: public detail::facade_base_ns::facade_base<Container, Derived>::type {};

} // namespace arx

#endif // __ARX_COLLECTIONS_FACADE_H__
