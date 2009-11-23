#ifndef __ARX_COLLECTIONS_STL_H__
#define __ARX_COLLECTIONS_STL_H__

#include "config.h"

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <boost/array.hpp>

#include "ContinuousSequenceFacade.h"
#include "OrderedSetFacade.h"
#include "BackSequenceFacadeMixin.h"
#include "SetObserverMixin.h"
#include "MapObserverMixin.h"
#include "MapFacadeMixin.h"

namespace arx {
// -------------------------------------------------------------------------- //
// vector
// -------------------------------------------------------------------------- //
  template<class T, class Allocator = std::allocator<T> >
  class vector: public BackSequenceFacadeMixin<ContinuousSequenceFacade<vector<T, Allocator>, std::vector<T, Allocator> > > {
    ARX_INHERIT_FACADE_BASE(BackSequenceFacadeMixin<ContinuousSequenceFacade<vector<T, Allocator>, std::vector<T, Allocator> > >);
  public:
    ARX_INJECT_TYPES(base_type, (size_type)(value_type)(allocator_type));

    vector() {}
    
    explicit vector(const allocator_type& allocator): mPrivate(allocator) {}
    
    explicit vector(size_type count): mPrivate(count) {}

    vector(size_type count, const value_type& value): mPrivate(count, value) {}

    vector(size_type count, const value_type& value, const allocator_type& allocator): mPrivate(count, value, allocator) {}
    
    vector(const vector& other): mPrivate(other.mPrivate) {}

    vector(const container_type& other): mPrivate(other) {}

    template<class InputIterator>
    vector(InputIterator first, InputIterator last): mPrivate(first, last) {}
    
    template<class InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type& allocator): mPrivate(first, last, allocator) {}
    
  protected:
    const container_type& crtpContainer() const {
      return mPrivate;
    }

    container_type& crtpContainer() {
      return mPrivate;
    }

  private:
    friend class FacadeBase<vector, container_type>;
    container_type mPrivate;
  };


// -------------------------------------------------------------------------- //
// map
// -------------------------------------------------------------------------- //
  template<class Key, class Type, class Cmp = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Type> > >
  class map: public MapObserverMixin<MapFacadeMixin<OrderedSetFacade<map<Key, Type, Cmp, Allocator>, std::map<Key, Type, Cmp, Allocator> > > > {
    ARX_INHERIT_FACADE_BASE(MapObserverMixin<MapFacadeMixin<OrderedSetFacade<map<Key, Type, Cmp, Allocator>, std::map<Key, Type, Cmp, Allocator> > > >);
  public:
    ARX_INJECT_TYPES(base_type, (key_compare)(allocator_type));

    map() {}

    explicit map(const key_compare& cmp): mPrivate(cmp) {}

    map(const key_compare& cmp, const allocator_type& allocator): mPrivate(cmp, allocator) {}

    map(const map& other): mPrivate(other.mPrivate) {}

    map(const container_type& other): mPrivate(other) {}

    template<class InputIterator>
    map(InputIterator first, InputIterator last): mPrivate(first, last) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& cmp): mPrivate(first, last, cmp) {}
    
    template<class InputIterator>
    map(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& allocator): mPrivate(first, last, cmp, allocator) {}

  protected:
    const container_type& crtpContainer() const {
      return mPrivate;
    }

    container_type& crtpContainer() {
      return mPrivate;
    }

  private:
    friend class FacadeBase<map, container_type>;
    container_type mPrivate;
  };



// -------------------------------------------------------------------------- //
// set
// -------------------------------------------------------------------------- //
  template<class Key, class Cmp = std::less<Key>, class Allocator = std::allocator<Key> >
  class set: public SetObserverMixin<OrderedSetFacade<set<Key, Cmp, Allocator>, std::set<Key, Cmp, Allocator> > > {
    ARX_INHERIT_FACADE_BASE(SetObserverMixin<OrderedSetFacade<set<Key, Cmp, Allocator>, std::set<Key, Cmp, Allocator> > >);
  public:
    ARX_INJECT_TYPES(base_type, (key_compare)(allocator_type));

    set() {}
    
    explicit set(const Cmp& cmp): mPrivate(cmp) {}

    set(const key_compare& cmp, const allocator_type& allocator): mPrivate(cmp, allocator) {}

    set(const set& other): mPrivate(other.mPrivate) {}

    set(const container_type& other): mPrivate(other) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last): mPrivate(first, last) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const key_compare& cmp): mPrivate(first, last, cmp) {}

    template<class InputIterator>
    set(InputIterator first, InputIterator last, const key_compare& cmp, const allocator_type& allocator): mPrivate(first, last, cmp, allocator) {}

  protected:
    const container_type& crtpContainer() const {
      return mPrivate;
    }

    container_type& crtpContainer() {
      return mPrivate;
    }

  private:
    friend class FacadeBase<set, container_type>;
    container_type mPrivate;
  };


// -------------------------------------------------------------------------- //
// array
// -------------------------------------------------------------------------- //
  using boost::array;


}


#endif // __ARX_COLLECTIONS_STL_H__
