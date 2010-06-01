#ifndef ARX_HASH_HACK_H
#define ARX_HASH_HACK_H

#include <typeindex>

/* Boost doesn't provide a hash function for std::type_index, so we have to do 
 * in ourselves. 
 *
 * We're injecting code into std namespace here, which is bad. */
namespace std {
  size_t hash_value(const type_index& typeIndex) {
    return hash<type_index>()(typeIndex);
  }

} // namespace std

#endif // ARX_HASH_HACK_H
