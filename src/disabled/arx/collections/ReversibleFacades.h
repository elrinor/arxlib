#ifndef __ARX_COLLECTIONS_REVERSIBLE_FACADES_H__
#define __ARX_COLLECTIONS_REVERSIBLE_FACADES_H__

#include "config.h"
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include "Injection.h"
#include "FacadeBase.h"

namespace arx {
// -------------------------------------------------------------------------- //
// WithReverseFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithReverseFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(container_type, (reverse_iterator)(const_reverse_iterator));

    reverse_iterator rbegin() {
      return this->container().rbegin();
    }

    const_reverse_iterator rbegin() const {
      return this->container().rbegin();
    }

    reverse_iterator rend() {
      return this->container().rend();
    }

    const_reverse_iterator rend() const {
      return this->container().rend();
    }
  };

  namespace detail {
    template<class T>
    struct is_implicitly_reversible: public boost::mpl::and_<has_reverse_iterator<T>, has_const_reverse_iterator<T>, has_rbegin<T>, has_rend<T> > {};
  } // namespace detail

  ARX_REGISTER_FACADE_IN_ARX_NS(WithReverseFacade, is_implicitly_reversible<T>)


// -------------------------------------------------------------------------- //
// WithoutReverseFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutReverseFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    typedef typename boost::range_reverse_iterator<container_type>::type reverse_iterator;
    typedef typename boost::range_const_reverse_iterator<container_type>::type const_reverse_iterator;

    reverse_iterator rbegin() {
      return boost::rbegin(this->container());
    }

    const_reverse_iterator rbegin() const {
      return boost::rbegin(this->container());
    }

    reverse_iterator rend() {
      return boost::rend(this->container());
    }

    const_reverse_iterator rend() const {
      return boost::rend(this->container());
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutReverseFacade, 
    boost::mpl::and_<
      boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, boost::bidirectional_traversal_tag>,
      boost::mpl::not_<is_implicitly_reversible<T> >
    >
  )

} // namespace arx

#endif // __ARX_COLLECTIONS_REVERSIBLE_FACADES_H__
