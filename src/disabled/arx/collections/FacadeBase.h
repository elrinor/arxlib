#ifndef __ARX_COLLECTIONS_FACADE_BASE_H__
#define __ARX_COLLECTIONS_FACADE_BASE_H__

#include "config.h"
#include "Injection.h"
#include "Traits.h"
#include "../StaticCounter.h"
#include "../HasXxx.h"

namespace arx {
  namespace detail {
    template<int n>
    struct RegisteredFacade;

    ARX_NEW_COUNTER(facade_count);
  } // namespace detail


// -------------------------------------------------------------------------- //
// FacadeBase
// -------------------------------------------------------------------------- //
  template<class Container, class Derived>
  class FacadeBase {
  protected:
    typedef Container container_type;
    typedef Derived derived_type;

    container_type& container() {
      return derived().container();
    }

    const container_type& container() const {
      return derived().container();
    }

    derived_type& derived() {
      return static_cast<derived_type&>(*this);
    }

    const derived_type& derived() const {
      return static_cast<const derived_type&>(*this);
    }
  };

} // namespace arx


// -------------------------------------------------------------------------- //
// Helpers
// -------------------------------------------------------------------------- //
#define ARX_INHERITED_FACADE(from)                                              \
  protected:                                                                    \
    ARX_INJECT_TYPES(from, (derived_type)(container_type))

#define ARX_INHERITED_FACADE_TPL(from)                                          \
  protected:                                                                    \
    ARX_INJECT_TYPES_TPL(from, (derived_type)(container_type))


// -------------------------------------------------------------------------- //
// Registration
// -------------------------------------------------------------------------- //
#define ARX_REGISTER_FACADE(name, ... /* requirements */)                       \
namespace arx {                                                                 \
  ARX_REGISTER_FACADE_IN_ARX_NS(name, __VA_ARGS__)                              \
}

#define ARX_REGISTER_FACADE_IN_ARX_NS(name, ... /* requirements */)             \
  namespace detail {                                                            \
    template<>                                                                  \
    struct RegisteredFacade<ARX_GET_COUNTER(facade_count)> {                    \
      template<class Base>                                                      \
      struct apply {                                                            \
        typedef name<Base> type;                                                \
      };                                                                        \
                                                                                \
      template<class T>                                                         \
      struct applicable_to {                                                    \
        typedef __VA_ARGS__ type;                                               \
      };                                                                        \
    };                                                                          \
                                                                                \
    ARX_INC_COUNTER(facade_count);                                              \
  }


#endif // __ARX_COLLECTIONS_FACADE_BASE_H__
