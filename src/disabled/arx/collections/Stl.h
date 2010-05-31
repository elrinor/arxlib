#ifndef __ARX_COLLECTIONS_STL_H__
#define __ARX_COLLECTIONS_STL_H__

#include "config.h"
#include <vector>
#include <set>
#include <map>
#include <boost/array.hpp>
#include "Injection.h"
#include "Extender.h"

namespace arx {
// -------------------------------------------------------------------------- //
// vector
// -------------------------------------------------------------------------- //
  template<class T, class Allocator = std::allocator<T> >
  class vector: public Extender<std::vector<T, Allocator>, vector<T, Allocator> > {
    typedef Extender<std::vector<T, Allocator>, vector<T, Allocator> > base_type;
    typedef std::vector<T, Allocator> container_type;

  public:
    ARX_INJECT_TYPES_TPL(container_type, (size_type)(value_type)(allocator_type));

    vector() {}
    
    explicit vector(const allocator_type& allocator): base_type(allocator) {}
    
    explicit vector(size_type count): base_type(count) {}

    vector(size_type count, const value_type& value): base_type(count, value) {}

    vector(size_type count, const value_type& value, const allocator_type& allocator): base_type(count, value, allocator) {}
    
    vector(const vector& other): base_type(other) {}

    vector(const container_type& other): base_type(other) {}

    template<class InputIterator>
    vector(InputIterator first, InputIterator last): base_type(first, last) {}
    
    template<class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& allocator): base_type(first, last, allocator) {}
  };


// -------------------------------------------------------------------------- //
// map
// -------------------------------------------------------------------------- //
  template<class Key, class Type, class Cmp = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Type> > >
  class map: public Extender<std::map<Key, Type, Cmp, Allocator>, map<Key, Type, Cmp, Allocator> > {
    typedef Extender<std::map<Key, Type, Cmp, Allocator>, map<Key, Type, Cmp, Allocator> > base_type;
    typedef std::map<Key, Type, Cmp, Allocator> container_type; 

  public:
    ARX_INJECT_TYPES_TPL(container_type, (key_compare)(allocator_type));

    map() {}

    explicit map(const key_compare& cmp): base_type(cmp) {}

    map(const key_compare& cmp, const allocator_type& allocator): base_type(cmp, allocator) {}

    map(const map& other): base_type(other) {}

    map(const container_type& other): base_type(other) {}

    template<class InputIterator>
    map(InputIterator first, InputIterator last): base_type(first, last) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& cmp): base_type(first, last, cmp) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& allocator): base_type(first, last, cmp, allocator) {}
  };


// -------------------------------------------------------------------------- //
// set
// -------------------------------------------------------------------------- //
  template<class Key, class Cmp = std::less<Key>, class Allocator = std::allocator<Key> >
  class set: public Extender<std::set<Key, Cmp, Allocator>, set<Key, Cmp, Allocator> > {
    typedef Extender<std::set<Key, Cmp, Allocator>, set<Key, Cmp, Allocator> > base_type;
    typedef std::set<Key, Cmp, Allocator> container_type; 
    
  public:
    ARX_INJECT_TYPES_TPL(container_type, (key_compare)(allocator_type));

    set() {}
    
    explicit set(const Cmp& cmp): base_type(cmp) {}

    set(const key_compare& cmp, const allocator_type& allocator): base_type(cmp, allocator) {}

    set(const set& other): base_type(other.mPrivate) {}

    set(const container_type& other): base_type(other) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last): base_type(first, last) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const key_compare& cmp): base_type(first, last, cmp) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& allocator): base_type(first, last, cmp, allocator) {}
  };


// -------------------------------------------------------------------------- //
// array
// -------------------------------------------------------------------------- //
  using boost::array;

}


#endif // __ARX_COLLECTIONS_STL_H__
