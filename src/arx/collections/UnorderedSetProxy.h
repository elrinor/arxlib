#ifndef __ARX_UNORDERDEDSETPROXY_H__
#define __ARX_UNORDERDEDSETPROXY_H__

#include "config.h"
#include "SetProxy.h"

namespace arx {
// -------------------------------------------------------------------------- //
// UnorderedSetProxy
// -------------------------------------------------------------------------- //
  template<class Derived, class Container>
  class UnorderedSetProxy: public SetProxy<Derived, Container> {
  public:
#define ARX_INJECT(T)                                                           \
    typedef typename container_type::T T
    ARX_INJECT(const_local_iterator);
    ARX_INJECT(local_iterator);
    ARX_INJECT(hasher);
    ARX_INJECT(key_equal);
#undef ARX_INJECT

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

#endif // __ARX_UNORDERDEDSETPROXY_H__
