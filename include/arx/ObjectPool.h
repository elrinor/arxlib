/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#ifndef ARX_OBJECT_POOL_H
#define ARX_OBJECT_POOL_H

#include <vector>
#include <algorithm>     /* for std::find that is used in assertions. */
#include <boost/noncopyable.hpp>

namespace arx {
  namespace detail {
// -------------------------------------------------------------------------- //
// AllocatorBase
// -------------------------------------------------------------------------- //
    /**
     * Empty base optimization.
     */
    template<class Allocator>
    class AllocatorBase {
    public:
      typedef Allocator allocator_type;
      typedef typename allocator_type::value_type      value_type;
      typedef typename allocator_type::size_type       size_type;
      typedef typename allocator_type::pointer         pointer;
      typedef typename allocator_type::const_pointer   const_pointer;
      typedef typename allocator_type::reference       reference;
      typedef typename allocator_type::const_reference const_reference;

      AllocatorBase() {}

      AllocatorBase(allocator_type allocator): mAllocator(allocator) {}

      /* Default copy constructor is Ok. */

      /* Default assignment operator is Ok. */

      allocator_type get_allocator() const {
        return mAllocator;
      }

    protected:
      allocator_type mAllocator;
    };


// -------------------------------------------------------------------------- //
// DeinitializerBase
// -------------------------------------------------------------------------- //
    /**
     * Empty base optimization.
     */
    template<class Allocator, class Deinitializer>
    class DeinitializerBase: public AllocatorBase<Allocator> {
    public:
      typedef Deinitializer deinitializer_type;

      DeinitializerBase() {}

      DeinitializerBase(allocator_type allocator): 
        AllocatorBase(allocator) {}

      DeinitializerBase(allocator_type allocator, deinitializer_type deinitializer): 
        AllocatorBase(allocator), mDeinitializer(deinitializer) {}

      allocator_type get_deinitializer() const {
        return mDeinitializer;
      }

    protected:
      deinitializer_type mDeinitializer;
    };


// -------------------------------------------------------------------------- //
// ObjectPoolBase
// -------------------------------------------------------------------------- //
    /**
     * Empty base optimization.
     */
    template<class Allocator, class Deinitializer, class Constructor>
    class ObjectPoolBase: public DeinitializerBase<Allocator, Deinitializer> {
    public:
      typedef Constructor constructor_type;

      ObjectPoolBase() {}

      ObjectPoolBase(allocator_type allocator): DeinitializerBase(allocator) {}

      ObjectPoolBase(allocator_type allocator, deinitializer_type deinitializer): 
        DeinitializerBase(allocator, deinitializer) {}

      ObjectPoolBase(allocator_type allocator, deinitializer_type deinitializer, constructor_type constructor): 
        DeinitializerBase(allocator, deinitializer), mConstructor(constructor) {}

      constructor_type get_constructor() const {
        return mConstructor;
      }

    protected:
      constructor_type mConstructor;
    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// deinitializer
// -------------------------------------------------------------------------- //
  template<class T>
  class deinitializer {
  public:
    void operator()(T* object) {
      object->deinitialize();
    }
  };


// -------------------------------------------------------------------------- //
// default_constructor
// -------------------------------------------------------------------------- //
  template<class T>
  class default_constructor {
  public:
    void operator()(T* ptr) {
      new (ptr) T();
    }
  };


// -------------------------------------------------------------------------- //
// ObjectPool
// -------------------------------------------------------------------------- //
  /** 
   * ObjectPool is not an allocator, so its interface differs. 
   */
  template<class T, class Allocator = std::allocator<T>, class Deinitializer = deinitializer<T>, class Constructor = default_constructor<T> > 
  class ObjectPool: public detail::ObjectPoolBase<Allocator, Deinitializer, Constructor>, public boost::noncopyable {
  public:
    ObjectPool() {}

    ObjectPool(allocator_type allocator): 
      ObjectPoolBase(allocator) {}

    ObjectPool(allocator_type allocator, deinitializer_type deinitializer): 
      ObjectPoolBase(allocator, deinitializer) {}

    ObjectPool(allocator_type allocator, deinitializer_type deinitializer, constructor_type constructor): 
      ObjectPoolBase(allocator, deinitializer, constructor) {}

    pointer get() {
      if(!mFree.empty()) {
        pointer result = mFree.back();
        mFree.pop_back();
        mDeinitializer(result);
        return result;
      } else {
        pointer result = mAllocator.allocate(1, NULL);
        mConstructor(result);
        mOwned.push_back(result);
        return result;
      }
    }

    void put(pointer object) {
      assert(std::find(mOwned.begin(), mOwned.end(), object) != mOwned.end()); /* It must belong to this ObjectPool. */
      assert(std::find(mFree.begin(), mFree.end(), object) == mFree.end()); /* It must be not free. */

      mFree.push_back(object);
    }

    ~ObjectPool() {
      for(int i = 0; i < mOwned.size(); i++) {
        mAllocator.destroy(mOwned[i]);
        mAllocator.deallocate(mOwned[i], 1);
      }
    }

  private:
    std::vector<pointer> mFree;   /**< Objects that can be deinitialized and returned with a call to get(). */
    std::vector<pointer> mOwned;  /**< All owned objects. */
  };

} // namespace arx

#endif // ARX_OBJECT_POOL_H
