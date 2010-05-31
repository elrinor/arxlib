#ifndef __ARX_COLLECTIONS_BACK_FRONT_FACADES_H__
#define __ARX_COLLECTIONS_BACK_FRONT_FACADES_H__

#include "config.h"
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// WithBackFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithBackFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference));

    reference back() {
      return this->container().back();
    }

    const_reference back() const {
      return this->container().back();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithBackFacade, boost::mpl::and_<has_back<T> >)


// -------------------------------------------------------------------------- //
// WithFrontFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithFrontFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference));

    reference front() {
      return this->container().front();
    }

    const_reference front() const {
      return this->container().front();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithFrontFacade, boost::mpl::and_<has_front<T> >)


// -------------------------------------------------------------------------- //
// WithoutBackFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutBackFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference)(iterator)(const_iterator));

    reference back() {
      iterator i = this->end();
      i--;
      return *i;
    }

    const_reference back() const {
      const_iterator i = this->end();
      i--;
      return *i;
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutBackFacade, 
    boost::mpl::and_<
      boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, boost::bidirectional_traversal_tag>,
      boost::mpl::not_<has_back<T> >,
      boost::mpl::not_<has_key_type<T> > /* Not for associative containers. */
    >
  )


// -------------------------------------------------------------------------- //
// WithoutFrontFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutFrontFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference)(iterator)(const_iterator));

    reference front() {
      return *this->begin();
    }

    const_reference front() const {
      return *this->begin();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutFrontFacade, 
    boost::mpl::and_<
      boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, boost::forward_traversal_tag>,
      boost::mpl::not_<has_front<T> >,
      boost::mpl::not_<has_key_type<T> > /* Not for associative containers. */
    >
  )


} // namespace arx

#endif // __ARX_COLLECTIONS_BACK_FRONT_FACADES_H__
