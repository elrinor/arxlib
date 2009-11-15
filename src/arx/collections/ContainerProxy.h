#ifndef __ARX_CONTAINERPROXY_H__
#define __ARX_CONTAINERPROXY_H__

#include "config.h"
#include "ProxyBase.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ContainerProxy
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class ContainerProxy: public ProxyBase<Derived, Container> {
  public:
#define ARX_INJECT(T)                                                           \
    typedef typename container_type::T T
    ARX_INJECT(allocator_type);
    ARX_INJECT(const_iterator);
    ARX_INJECT(const_pointer);
    ARX_INJECT(const_reference);
    ARX_INJECT(const_reverse_iterator);
    ARX_INJECT(difference_type);
    ARX_INJECT(iterator);
    ARX_INJECT(pointer);
    ARX_INJECT(reference);
    ARX_INJECT(reverse_iterator);
    ARX_INJECT(size_type);
    ARX_INJECT(value_type);
#undef ARX_INJECT

    const_iterator begin() const {
      return container().begin();
    }

    iterator begin() {
      return container().begin();
    }

    void clear() const {
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
      swap(l.container(), r.container());
    }

    friend bool operator== (const derived_type& l, const derived_type& r) {
      return l.container() == r.container();
    }

    friend bool operator!= (const derived_type& l, const derived_type& r) {
      return l.container() != r.container();
    }

    friend bool operator< (const derived_type& l, const derived_type& r) {
      return l.container() < r.container();
    }

    friend bool operator> (const derived_type& l, const derived_type& r) {
      return l.container() > r.container();
    }

    friend bool operator<= (const derived_type& l, const derived_type& r) {
      return l.container() <= r.container();
    }

    friend bool operator>= (const derived_type& l, const derived_type& r) {
      return l.container() >= r.container();
    }

    /* Additional mutators. */

    derived_type& operator<< (const value_type& value) {
      insert(end(), value);
      return derived();
    }
  };

} // namespace arx

#endif // __ARX_CONTAINERPROXY_H__
