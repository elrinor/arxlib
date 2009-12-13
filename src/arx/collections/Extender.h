#ifndef __ARX_COLLECTIONS_EXTENDER_H__
#define __ARX_COLLECTIONS_EXTENDER_H__

#include "config.h"
#include <boost/range.hpp> /* for boost::range_value */
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/identity.hpp>
#include "ExtenderBase.h"
#include "Forward.h"

/* Register extenders. */
#include "PushBackExtender.h"
#include "PushFrontExtender.h"
#include "WriteExtender.h"
#include "MapBrackExtender.h"
#include "ContainsExtender.h"

namespace arx {
  namespace detail {
    namespace extender_base_ns {
      using namespace boost::mpl;
// -------------------------------------------------------------------------- //
// extender_base
// -------------------------------------------------------------------------- //
      template<class Base, class Derived, long n = ARX_GET_COUNTER(extender_count) - 1>
      struct extender_base {
        typedef typename extender_base<Base, Derived, n - 1>::type current_base;

        typedef typename eval_if<typename RegisteredExtender<n>::template applicable_to<Base>::type,
          typename RegisteredExtender<n>::template apply<current_base>,
          identity<current_base>
        >::type type;
      };

      template<class Base, class Derived>
      struct extender_base<Base, Derived, -1> {
        typedef ExtenderBase<Base, Derived> type;
      };

    } // namespace extender_base_ns
  } // namespace detail


// -------------------------------------------------------------------------- //
// Extender
// -------------------------------------------------------------------------- //
  template<class Base, class Derived>
  class Extender: public detail::extender_base_ns::extender_base<Base, Derived>::type {
    typedef typename detail::extender_base_ns::extender_base<Base, Derived>::type base_type;

  public:
    ARX_FORWARD_CONSTRUCTOR(Extender, (ARX_FWD_TEXT(base_type))(ARX_FWD_ARGS())(ARX_FWD_TEXT({})));

  };

} // namespace arx


#endif // __ARX_COLLECTIONS_EXTENDER_H__
