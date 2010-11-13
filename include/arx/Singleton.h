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
 * $$ */
#ifndef ARX_SINGLETON_H
#define ARX_SINGLETON_H

#include "config.h"
#include <cassert>
#include <boost/noncopyable.hpp>

namespace arx {
// -------------------------------------------------------------------------- //
// SingletonFactory
// -------------------------------------------------------------------------- //
  /**
   * SingletonFactory is a class that performs construction and destruction of
   * a singleton instance. 
   * 
   * User should befriend this class if he wants to use Singleton with a class
   * that declares a private of protected constructor or destructor.
   */
  class SingletonFactory {
  public:
    template<class T>
    static void create(void *position) {
      new (position) T();
    }

    template<class T>
    static void destroy(void *position) {
      static_cast<T *>(position)->~T();
    }

  };


  namespace detail {
    template<class T>
    class SingletonGuard {
    public:
      SingletonGuard() {
        SingletonFactory::create<T>(mStorage);
      }

      ~SingletonGuard() {
        SingletonFactory::destroy<T>(mStorage);
        sIsDestroyed = true;
      }

      static bool isDestroyed() {
        return sIsDestroyed;
      }

      T &instance() {
        return reinterpret_cast<T &>(mStorage);
      }

    private:
      static bool sIsDestroyed;
      char mStorage[sizeof(T)];
    };
  
    template<class T> 
    bool SingletonGuard<T>::sIsDestroyed = false;

  } // namespace detail


// -------------------------------------------------------------------------- //
// Singleton
// -------------------------------------------------------------------------- //
  /**
   * Singleton class implements a singleton pattern. That is, for a given
   * template parameter T, only one instance of class T is created.
   */
  template<class T>
  class Singleton: public boost::noncopyable {
  public:
    static T &instance() {
      static detail::SingletonGuard<T> guard;

      assert(!guard.isDestroyed());

      /* Use sInstance to instantiate it and force initialization at startup 
       * time. */
      use(sInstance);

      return guard.instance();
    }

  private:
    static void use(T &) {}

    static T &sInstance;
  };

  template<class T> 
  T &Singleton<T>::sInstance = Singleton<T>::instance();

} // namespace arx

#endif // ARX_SINGLETON_H
