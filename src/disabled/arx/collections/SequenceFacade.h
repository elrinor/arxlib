#ifndef __ARX_COLLECTIONS_SEQUENCE_FACADE_H__
#define __ARX_COLLECTIONS_SEQUENCE_FACADE_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// SequenceFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class SequenceFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (size_type)(value_type)(reference)(const_reference)(iterator));

    /* Required members. */

    void assign(size_type count, const value_type& value) {
      this->container().assign(count, value);
    }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last) {
      this->container().assign(first, last);
    }

    void clear() {
      this->container().clear();
    }

    iterator erase(iterator where) {
      this->container.erase(where);
    }

    iterator erase(iterator first, iterator last) {
      this->container.erase(first, last);
    }

    void insert(iterator where, const value_type& value) {
      return this->container().insert(where, value);
    }

    void insert(iterator where, size_type count, const value_type& value) {
      return this->container().insert(where, count, value);
    }

    template<class InputIterator>
    void insert(iterator where, InputIterator first, InputIterator last) {
      return container().insert(where, first, last);
    }


    /* Optional members. */
    void resize(size_type newSize) {
      this->container().resize(newSize);
    }

    void resize(size_type newSize, const value_type& value) {
      this->container().resize(newSize, value);
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(SequenceFacade, boost::mpl::and_<has_assign<T>, has_erase<T>, has_insert<T>, has_clear<T>, /* and just in case */ boost::mpl::not_<has_key_type<T> > >);

} // namespace arx

#endif // __ARX_COLLECTIONS_SEQUENCE_FACADE_H__
