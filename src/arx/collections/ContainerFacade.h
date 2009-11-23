#ifndef __ARX_COLLECTIONS_CONTAINER_FACADE_H__
#define __ARX_COLLECTIONS_CONTAINER_FACADE_H__

#include "config.h"
#include "FacadeBase.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ContainerFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class ContainerFacade: public FacadeBase<Derived, Container> {
    ARX_INHERIT_FACADE_BASE(FacadeBase<Derived, Container>);
    typedef ContainerFacade<Derived, Container> this_type;
  public:
    ARX_INJECT_TYPES(container_type, (allocator_type)(const_iterator)(const_pointer)(const_reference)(const_reverse_iterator)(difference_type)(iterator)(pointer)(reference)(reverse_iterator)(size_type)(value_type));

    const_iterator begin() const {
      return container().begin();
    }

    iterator begin() {
      return container().begin();
    }

    void clear() {
      container().clear();
    }

    bool empty() const {
      return container().empty();
    }

    iterator end() {
      return container().end();
    }

    const_iterator end() const {
      return container().end();
    }

    iterator erase(iterator where) {
      return container().erase(where);
    }

    iterator erase(iterator first, iterator last) {
      return container().erase(first, last);
    }

    allocator_type get_allocator() const {
      return container().get_allocator();
    }

    iterator insert(iterator where, const value_type& value) {
      return container().insert(where, value);
    }

    size_type max_size() const {
      return container().max_size();
    }

    reverse_iterator rbegin() {
      return container().rbegin();
    }

    const_reverse_iterator rbegin() const {
      return container().rbegin();
    }

    reverse_iterator rend() {
      return container().rend();
    }

    const_reverse_iterator rend() const {
      return container().rend();
    }

    size_type size() const {
      return container().size();
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

    /* Additional mutators. */

    derived_type& operator<< (const value_type& value) {
      insert(end(), value);
      return derived();
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_CONTAINER_FACADE_H__
