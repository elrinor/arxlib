#ifndef __ARX_VIEW_TRAITS_H__
#define __ARX_VIEW_TRAITS_H__

#include "config.h"
#include <boost/type_traits.hpp> /* for boost::is_convertible, boost::is_same */
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>

namespace arx {
  /** Tag for proxy types. */
  struct proxy_tag {};

  /** Tag for non-proxy types. */
  struct non_proxy_tag {};

  namespace detail {
    namespace proxy_category_ns {
      using namespace boost::mpl;

      BOOST_MPL_HAS_XXX_TRAIT_DEF(proxy_category);

      template<class T, class Tag> 
      struct is_proxy_convertible: public boost::is_convertible<typename T::proxy_category, Tag> {};

      template<class T>
      struct proxy_category_impl: public 
        if_<
          not_<has_proxy_category<T> >, 
          non_proxy_tag,
          if_<
            is_proxy_convertible<T, proxy_tag>,
            proxy_tag,
            non_proxy_tag
          >
        > {};
    } // namespace proxy_category_ns
  } // namespace detail

  /** Metafunction that returns proxy category of T. */
  template<class T>
  proxy_category: public detail::proxy_category_ns::proxy_category_impl<T> {};

  /** Metafunction that returns whether T is a proxy type. 
   * Proxy does not own the underlying data, therefore it can be copied around freely.*/
  template<class T>
  is_proxy: public boost::is_same<proxy_category<T>::type, proxy_tag> {};

} // namespace arx

#endif // __ARX_VIEW_TRAITS_H__
