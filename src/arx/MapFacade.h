#ifndef __ARX_MAPFACADE_H__
#define __ARX_MAPFACADE_H__

#include "config.h"
#include <cassert>
#include "RangeFacade.h"

#include <xhash>

namespace arx {
// -------------------------------------------------------------------------- //
// MapFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class MapFacade: public RangeFacade<Derived, Container> {
  public:
#define ARX_INJECT(T)                                                           \
    typedef typename container_type::T T
    ARX_INJECT(key_compare);
    ARX_INJECT(key_type);
    ARX_INJECT(value_compare);
    ARX_INJECT(mapped_type);
#undef ARX_INJECT

    size_type count(const key_type& key) const {
      return container().count(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
      return container().equal_range(key);
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
      return container().equal_range(key);
    }

    size_type erase(const key_type& key) {
      return container().erase(key);
    }

    iterator find(const key_type& key) {
      return container().find(key);
    }

    const_iterator find(const key_type& key) const {
      return container().find(key);
    }

    std::pair<iterator, bool> insert(const value_type& value) {
      return container().insert(value);
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
      return container().insert(first, last);
    }

    key_compare key_comp() const {
      return container().key_comp();
    }

    iterator lower_bound(const key_type& key) {
      return container().lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const {
      return container().lower_bound(key);
    }

    iterator upper_bound(const key_type& key) {
      return container().upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const {
      return container().upper_bound(key);
    }

    value_compare value_comp() const {
      return container().value_comp();
    }

    mapped_type& operator[] (const key_type& key) {
      return container().operator[] (key);
    }

    /* Additional functions. */

    bool contains(const key_type& key) const {
      return find(key) != end();
    }

    mapped_type& operator[] (const key_type& key) const {
      assert(contains(key));

      return find(key)->second;
    }

  protected:
    /*
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

  private:
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

} // namespace arx

#endif // __ARX_MAPFACADE_H__
