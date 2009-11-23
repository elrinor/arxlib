#ifndef __ARX_COLLECTIONS_FACADE_BASE_H__
#define __ARX_COLLECTIONS_FACADE_BASE_H__

#include "config.h"
#include <boost/preprocessor.hpp> /* for BOOST_PP_SEQ_FOR_EACH */

namespace arx {
// -------------------------------------------------------------------------- //
// FacadeBase
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class FacadeBase {
  protected:
    typedef FacadeBase facade_base_type;
    typedef Derived derived_type;
    typedef Container container_type;

  protected:    
    /* 
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

    container_type& container() {
      return derived().crtpContainer();
    }

    const container_type& container() const {
      return derived().crtpContainer();
    }

    derived_type& derived() {
      return static_cast<derived_type&>(*this);
    }

    const derived_type& derived() const {
      return static_cast<const derived_type&>(*this);
    }
  };


#define ARX_INHERIT_FACADE_BASE(...)                                            \
  protected:                                                                    \
    typedef __VA_ARGS__ base_type;                                              \
    typedef typename base_type::facade_base_type facade_base_type;              \
    typedef typename facade_base_type::container_type container_type;           \
    typedef typename facade_base_type::derived_type derived_type;               \
                                                                                \
    container_type& container() {                                               \
      return facade_base_type::container();                                     \
    }                                                                           \
                                                                                \
    const container_type& container() const {                                   \
      return facade_base_type::container();                                     \
    }                                                                           \
                                                                                \
    derived_type& derived() {                                                   \
      return facade_base_type::derived();                                       \
    }                                                                           \
                                                                                \
    const derived_type& derived() const {                                       \
      return facade_base_type::derived();                                       \
    }

#define ARX_INJECT_TYPES_I(r, from, e)                                          \
  typedef typename from::e e;

#define ARX_INJECT_TYPES(from, type_list)                                       \
  BOOST_PP_SEQ_FOR_EACH(ARX_INJECT_TYPES_I, from, type_list)

} // namespace arx

#endif // __ARX_COLLECTIONS_FACADE_BASE_H__
