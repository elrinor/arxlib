#ifndef __ARX_COLLECTIONS_INDEX_FACADES_H__
#define __ARX_COLLECTIONS_INDEX_FACADES_H__

#include "config.h"
#include <exception> /* for std::out_of_range */
#include <typeinfo>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// WithIndexFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithIndexFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference)(size_type));

    reference at(size_type pos) {
      return this->container().at(pos);
    }

    const_reference at(size_type pos) const {
      return this->container().at(pos);
    }

    reference operator[] (size_type pos) {
      return this->container().operator[] (pos);
    }

    const_reference operator[] (size_type pos) const {
      return this->container().operator[] (pos);
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithIndexFacade, boost::mpl::and_<has_operator_brack<T>, boost::mpl::not_<has_key_type<T> > /* not for associative */ >)


// -------------------------------------------------------------------------- //
// WithoutIndexFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class WithoutIndexFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (reference)(const_reference)(size_type));

    reference at(size_type pos) {
      if(this->size() <= pos || pos < 0)
        xRan();
      return operator[] (pos);
    }

    const_reference at(size_type pos) const {
      if(this->size() <= pos || pos < 0)
        xRan();
      return operator[] (pos);
    }

    reference operator[] (size_type pos) {
      return *(this->begin() + pos);
    }

    const_reference operator[] (size_type pos) const {
      return *(this->begin() + pos);
    }

  private:
    void xRan() const {
      ARX_ASSERT_OR_THROW(true, throw std::out_of_range(std::string() + "invalid " + typeid(derived_type).name() + " subscript"));
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(WithoutIndexFacade, 
    boost::mpl::and_<
      boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, boost::random_access_traversal_tag>,
      boost::mpl::not_<has_operator_brack<T> >,
      boost::mpl::not_<has_key_type<T> >
    >
  )

} // namespace arx

#endif // __ARX_COLLECTIONS_INDEX_FACADES_H__
