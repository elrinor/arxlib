#ifndef __ARX_COLLECTIONS_SET_FACADE_H__
#define __ARX_COLLECTIONS_SET_FACADE_H__

#include "config.h"
#include <boost/range.hpp>  /* for boost::begin & boost::end. */
#include "ContainerFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// SetFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class SetFacade: public ContainerFacade<Derived, Container> {
  public:
#define ARX_INJECT(T)                                                           \
    typedef typename container_type::T T
    ARX_INJECT(key_compare);
    ARX_INJECT(key_type);
    ARX_INJECT(value_compare);
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

    /* Additional mutators. */

    template<class InputRange>
    void insert(InputRange range) {
      insert(boost::begin(range), boost::end(range));
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_SET_FACADE_H__
