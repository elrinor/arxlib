#ifndef __ARX_COLLECTIONS_UNORDERDED_SET_FACADE_H__
#define __ARX_COLLECTIONS_UNORDERDED_SET_FACADE_H__

#include "config.h"
#include "SetFacade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// UnorderedSetFacade
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class UnorderedSetFacade: public SetFacade<Derived, Container> {
    ARX_INHERIT_FACADE_BASE(SetFacade<Derived, Container>);
  public:
    ARX_INJECT_TYPES(container_type, (const_local_iterator)(local_iterator)(hasher)(key_equal))

    size_type bucket(const key_type& key) const {
      return container().bucket(key);
    }

    size_type bucket_count() const {
      return container().bucket_count();
    }

    size_type bucket_size(size_type nbucket) const {
      return container().bucket_size(nbucket);
    }

    hasher hash_function() const {
      return container().hash_function();
    }

    key_equal key_eq() const {
      return container().key_eq();
    }

    float load_factor() const {
      return container().load_factor();
    }

    size_type max_bucket_count() const {
      return container().max_bucket_count();
    }

    float max_load_factor() const {
      return container().max_load_factor();
    }

    void max_load_factor(float factor) {
      container().max_load_factor(factor);
    }

    void rehash(size_type nbuckets) {
      container().rehash(nbuckets);
    }
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_UNORDERDED_SET_FACADE_H__
