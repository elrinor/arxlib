#ifndef __ARX_COLLECTIONS_SEQUENCE_FACADE_H__
#define __ARX_COLLECTIONS_SEQUENCE_FACADE_H__

#include "config.h"
#include <boost/range.hpp>  /* for boost::begin & boost::end. */
#include "ContainerFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// SequenceFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class SequenceFacade: public ContainerFacade<Derived, Container> {
    ARX_INHERIT_FACADE_BASE(ContainerFacade<Derived, Container>);
  public:
    ARX_INJECT_TYPES(base_type, (size_type)(value_type)(reference)(const_reference)(iterator));

    void assign(size_type count, const value_type& value) {
      container().assign(count, value);
    }

    template<class InputIterator>
    void assign(InputIterator first, InputIterator last) {
      container().assign(first, last);
    }

    reference back() {
      return container().back();
    }

    const_reference back() const {
      return container().back();
    }

    reference front() {
      return container().front();
    }

    const_reference front() const {
      return container().front();
    }

    using base_type::insert;

    void insert(iterator where, size_type count, const value_type& value) {
      return container().insert(where, count, value);
    }

    template<class InputIterator>
    void insert(iterator where, InputIterator first, InputIterator last) {
      return container().insert(where, first, last);
    }

    void resize(size_type newSize) {
      container().resize(newSize);
    }

    void resize(size_type newSize, const value_type& value) {
      container().resize(newSize, value);
    }

    /* Additional mutators. */

    template<class InputRange>
    void insert(iterator where, InputRange range) {
      insert(where, boost::begin(range), boost::end(range));
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_SEQUENCE_FACADE_H__
