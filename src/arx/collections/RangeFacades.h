#ifndef __ARX_COLLECTIONS_RANGE_FACADES_H__
#define __ARX_COLLECTIONS_RANGE_FACADES_H__

#include "config.h"
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/bool.hpp> /* for boost::mpl::true_ */
#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include "FacadeBase.h"

namespace arx {
// -------------------------------------------------------------------------- //
// WithEmptyFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithEmptyFacade: public Base {
  public:
    bool empty() const {
      return this->container().empty();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithEmptyFacade, has_empty<T>)


// -------------------------------------------------------------------------- //
// WithoutEmptyFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutEmptyFacade: public Base {
  public:
    bool empty() const {
      return boost::empty(this->container());
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutEmptyFacade, boost::mpl::not_<has_empty<T> >)


// -------------------------------------------------------------------------- //
// WithSizeFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithSizeFacade: public Base {
  public:
    typename Base::container_type::size_type size() const {
      return this->container().size();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithSizeFacade, boost::mpl::and_<has_size<T>, has_size_type<T> >)


// -------------------------------------------------------------------------- //
// WithoutSizeFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutSizeFacade: public Base {
  public:
    typename boost::range_size<typename Base::container_type>::type size() const {
      return boost::size(this->container());
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutSizeFacade, 
    boost::mpl::and_<
      boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, boost::random_access_traversal_tag>,
      boost::mpl::not_<boost::mpl::and_<has_size<T>, has_size_type<T> > >
    >
  )


// -------------------------------------------------------------------------- //
// RangeFacade
// -------------------------------------------------------------------------- //
  /**
   * Facade for classes satisfying range requirements.
   */
  template<class Base>
  class RangeFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base)
  public:
    typedef typename boost::range_difference<container_type>::type     difference_type;
    typedef typename boost::range_size<container_type>::type           size_type;
    typedef typename boost::range_value<container_type>::type          value_type;
    typedef typename boost::range_iterator<container_type>::type       iterator;
    typedef typename boost::range_const_iterator<container_type>::type const_iterator;
    typedef typename boost::iterator_reference<iterator>::type         reference;
    typedef typename boost::iterator_reference<const_iterator>::type   const_reference;

    const_iterator begin() const {
      return boost::begin(this->container());
    }

    iterator begin() {
      return boost::begin(this->container());
    }

    iterator end() {
      return boost::end(this->container());
    }

    const_iterator end() const {
      return boost::end(this->container());
    }

    size_type max_size() const {
      return this->container().max_size();
    }

    void swap(derived_type& other) {
      container().swap(other);
    }

    friend void swap(derived_type& l, derived_type& r) {
      swap(l.this_type::container(), r.this_type::container());
    }

    friend bool operator== (const derived_type& l, const derived_type& r) {
      return l.this_type::container() == r.this_type::container();
    }

    friend bool operator!= (const derived_type& l, const derived_type& r) {
      return l.this_type::container() != r.this_type::container();
    }

    friend bool operator< (const derived_type& l, const derived_type& r) {
      return l.this_type::container() < r.this_type::container();
    }

    friend bool operator> (const derived_type& l, const derived_type& r) {
      return l.this_type::container() > r.this_type::container();
    }

    friend bool operator<= (const derived_type& l, const derived_type& r) {
      return l.this_type::container() <= r.this_type::container();
    }

    friend bool operator>= (const derived_type& l, const derived_type& r) {
      return l.this_type::container() >= r.this_type::container();
    }

    /* size() and empty() are handled by additional facades. */
  };

  /* It is presumed that container facade requirements are always satisfied. If not,
   * then handling the mess is up to the user. */
  ARX_REGISTER_FACADE_IN_ARX_NS(RangeFacade, boost::mpl::true_)

} // namespace arx

#endif // __ARX_COLLECTIONS_RANGE_FACADES_H__
