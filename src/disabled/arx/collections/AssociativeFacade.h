#ifndef __ARX_COLLECTIONS_ASSOCIATIVE_FACADE_H__
#define __ARX_COLLECTIONS_ASSOCIATIVE_FACADE_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
  // -------------------------------------------------------------------------- //
  // AssociativeFacade
  // -------------------------------------------------------------------------- //
  template<class Base>
  class AssociativeFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(container_type, (key_type)(key_compare)(value_compare));
    ARX_INJECT_TYPES_TPL(Base, (iterator)(const_iterator)(value_type)(size_type));

    void clear() {
      this->container().clear();
    }

    size_type count(const key_type& key) const {
      return this->container().count(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
      return this->container().equal_range(key);
    }

    std::pair<iterator, iterator> equal_range(const key_type& key) {
      return this->container().equal_range(key);
    }

    size_type erase(const key_type& key) {
      return this->container().erase(key);
    }

    iterator erase(iterator where) {
      this->container.erase(where);
    }

    iterator erase(iterator first, iterator last) {
      this->container.erase(first, last);
    }

    iterator find(const key_type& key) {
      return this->container().find(key);
    }

    const_iterator find(const key_type& key) const {
      return this->container().find(key);
    }

    std::pair<iterator, bool> insert(const value_type& value) {
      return this->container().insert(value);
    }

    /*iterator insert(const value_type& value) {
      return this->container().insert(value);
    }*/ /* TODO */

    iterator insert(iterator where, const value_type& value) {
      return this->container().insert(where, value);
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
      return this->container().insert(first, last);
    }

    key_compare key_comp() const {
      return this->container().key_comp();
    }

    iterator lower_bound(const key_type& key) {
      return this->container().lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const {
      return this->container().lower_bound(key);
    }

    iterator upper_bound(const key_type& key) {
      return this->container().upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const {
      return this->container().upper_bound(key);
    }

    value_compare value_comp() const {
      return this->container().value_comp();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(AssociativeFacade, boost::mpl::and_<has_key_type<T>, has_key_compare<T>, has_value_compare<T> >);

} // namespace arx

#endif // __ARX_COLLECTIONS_ASSOCIATIVE_FACADE_H__
