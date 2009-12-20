#ifndef __ARX_COLLECTIONS_UNORDERED_FACADE_H__
#define __ARX_COLLECTIONS_UNORDERED_FACADE_H__

#include "config.h"
#include <boost/mpl/and.hpp>
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// UnorderedFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class UnorderedFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(container_type, (key_type)(hasher)(key_equal)(local_iterator)(const_local_iterator));
    ARX_INJECT_TYPES_TPL(Base, (iterator)(const_iterator)(value_type)(size_type));

    size_type bucket(const key_type& key) const {
      return this->container().bucket(key);
    }

    size_type bucket_count() const {
      return this->container().bucket_count();
    }

    size_type bucket_size(size_type nbucket) const {
      return this->container().bucket_size(nbucket);
    }

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

    hasher hash_function() const {
      return this->container().hash_function();
    }

    std::pair<iterator, bool> insert(const value_type& value) {
      return this->container().insert(value);
    } /* TODO: insert for multimap has different return type. */

    iterator insert(iterator where, const value_type& value) {
      return this->container().insert(where, value);
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
      return this->container().insert(first, last);
    }

    key_equal key_eq() const {
      return this->container().key_eq();
    }

    float load_factor() const {
      return this->container().load_factor();
    }

    iterator lower_bound(const key_type& key) {
      return this->container().lower_bound(key);
    }

    const_iterator lower_bound(const key_type& key) const {
      return this->container().lower_bound(key);
    }

    size_type max_bucket_count() const {
      return this->container().max_bucket_count();
    }

    float max_load_factor() const {
      return this->container().max_load_factor();
    }

    void max_load_factor(float factor) {
      this->container().max_load_factor(factor);
    }

    void rehash(size_type nbuckets) {
      this->container().rehash(nbuckets);
    }

    iterator upper_bound(const key_type& key) {
      return this->container().upper_bound(key);
    }

    const_iterator upper_bound(const key_type& key) const {
      return this->container().upper_bound(key);
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(UnorderedFacade, boost::mpl::and_<has_key_type<T>, has_hasher<T>, has_key_equal<T>, has_local_iterator<T>, has_const_local_iterator<T> >);

} // namespace arx

#endif // __ARX_COLLECTIONS_UNORDERED_FACADE_H__
