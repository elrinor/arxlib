#ifndef __ARX_STL_H__
#define __ARX_STL_H__

#include "config.h"

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "RaSequenceProxy.h"
#include "OrderedSetProxy.h"
#include "BackSequenceProxyMixin.h"
#include "SetObserverMixin.h"
#include "MapObserverMixin.h"
#include "MapProxyMixin.h"

namespace arx {
// -------------------------------------------------------------------------- //
// vector
// -------------------------------------------------------------------------- //
  template<class T, class Allocator = std::allocator<T> >
  class vector: public BackSequenceProxyMixin<RaSequenceProxy<vector<T, Allocator>, std::vector<T, Allocator> > > {
  private:
    typedef BackSequenceProxyMixin base_type;

  public:
    vector() {}
    
    explicit vector(const Allocator& allocator): mPrivate(allocator) {}
    
    explicit vector(typename base_type::size_type count): mPrivate(count) {}

    vector(typename base_type::size_type count, const typename base_type::value_type& value): mPrivate(count, value) {}

    vector(typename base_type::size_type count, const typename base_type::value_type& value, const Allocator& allocator): mPrivate(count, value, allocator) {}
    
    vector(const vector<T, Allocator>& other): mPrivate(other.mPrivate) {}

    vector(const typename base_type::container_type& other): mPrivate(other) {}

    template<class InputIterator>
    vector(InputIterator first, InputIterator last): mPrivate(first, last) {}
    
    template<class InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator& allocator): mPrivate(first, last, allocator) {}
    
  protected:
    const std::vector<T, Allocator>& crtpContainer() const {
      return mPrivate;
    }

    std::vector<T, Allocator>& crtpContainer() {
      return mPrivate;
    }

  private:
    friend ProxyAccess;
    std::vector<T, Allocator> mPrivate;
  };


// -------------------------------------------------------------------------- //
// map
// -------------------------------------------------------------------------- //
  template<class Key, class Type, class Cmp = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Type> > >
  class map: public MapObserverMixin<MapProxyMixin<OrderedSetProxy<map<Key, Type, Cmp, Allocator>, std::map<Key, Type, Cmp, Allocator> > > > {
  private:
    typedef MapObserverMixin base_type;

  public:
    map() {}

    explicit map(const Cmp& cmp): mPrivate(cmp) {}

    map(const Cmp& cmp, const Allocator& allocator): mPrivate(cmp, allocator) {}

    map(const map<Key, Type, Cmp, Allocator>& other): mPrivate(other.mPrivate) {}

    map(const typename base_type::container_type& other): mPrivate(other) {}

    template<class InputIterator>
    map(InputIterator first, InputIterator last): mPrivate(first, last) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const Cmp& cmp): mPrivate(first, last, cmp) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const Cmp& cmp, const Allocator& allocator): mPrivate(first, last, cmp, allocator) {}

  protected:
    const std::map<Key, Type, Cmp, Allocator>& crtpContainer() const {
      return mPrivate;
    }

    std::map<Key, Type, Cmp, Allocator>& crtpContainer() {
      return mPrivate;
    }

  private:
    friend ProxyAccess;
    std::map<Key, Type, Cmp, Allocator> mPrivate;
  };



// -------------------------------------------------------------------------- //
// set
// -------------------------------------------------------------------------- //
  template<class Key, class Cmp = std::less<Key>, class Allocator = std::allocator<Key> >
  class set: public SetObserverMixin<OrderedSetProxy<set<Key, Cmp, Allocator>, std::set<Key, Cmp, Allocator> > > {
  private:
    typedef SetObserverMixin base_type;

  public:
    set() {}
    
    explicit set(const Cmp& cmp): mPrivate(cmp) {}

    set(const Cmp& cmp, const Allocator& allocator): mPrivate(cmp, allocator) {}

    set(const set<Key, Cmp, Allocator>& other): mPrivate(other.mPrivate) {}

    set(const typename base_type::container_type& other): mPrivate(other) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last): mPrivate(first, last) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const Cmp& cmp): mPrivate(first, last, cmp) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const Cmp& comp, const Allocator& allocator): mPrivate(first, last, cmp, allocator) {}

  protected:
    const std::set<Key, Cmp, Allocator>& crtpContainer() const {
      return mPrivate;
    }

    std::set<Key, Cmp, Allocator>& crtpContainer() {
      return mPrivate;
    }

  private:
    friend ProxyAccess;
    std::set<Key, Cmp, Allocator> mPrivate;
  };


}


#endif // __ARX_STL_H__
