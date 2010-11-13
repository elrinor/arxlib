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
#include <boost/mpl/map.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/at.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/interprocess/sync/null_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>


namespace arx {
  struct is_initialized_at_startup_t {};
  struct is_thread_safe_t {};

  typedef boost::mpl::map0<> default_singleton_policies;


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
// -------------------------------------------------------------------------- //
// SingletonStorage
// -------------------------------------------------------------------------- //
    template<class T>
    class SingletonStorage {
    public:
      SingletonStorage() {
        SingletonFactory::create<T>(mStorage);
      }

      ~SingletonStorage() {
        SingletonFactory::destroy<T>(mStorage);
        sIsDestroyed = true;
      }

      static bool isDestroyed() {
        return sIsDestroyed;
      }

      T &instance() {
        return reinterpret_cast<T &>(mStorage);
      }

      static T &staticInstance() {
        static SingletonStorage guard;

        assert(!guard.isDestroyed());

        return guard.instance();
      }

    private:
      static bool sIsDestroyed;
      char mStorage[sizeof(T)];
    };
  
    template<class T> 
    bool SingletonStorage<T>::sIsDestroyed = false;

  } // namespace detail


  namespace detail {
// -------------------------------------------------------------------------- //
// SingletonPolicies
// -------------------------------------------------------------------------- //
    template<class PolicyList>
    class SingletonPolicies {
    public:
      typedef typename boost::mpl::at<PolicyList, is_initialized_at_startup_t>::type  user_initialized_at_startup;
      typedef typename boost::mpl::at<PolicyList, is_thread_safe_t>::type             user_thread_safe;

      typedef boost::mpl::true_                                                       default_initialized_at_startup;
      typedef boost::mpl::true_                                                       default_thread_safe;

      typedef typename boost::mpl::if_<
        boost::is_same<user_initialized_at_startup, boost::mpl::void_>,
        default_initialized_at_startup,
        user_initialized_at_startup
      >::type initialized_at_startup;

      typedef typename boost::mpl::if_<
        boost::is_same<user_thread_safe, boost::mpl::void_>,
        default_thread_safe,
        user_thread_safe
      >::type thread_safe;

      typedef boost::mpl::and_<
        boost::mpl::not_<initialized_at_startup>,
        thread_safe
      > locked;

      typedef typename boost::mpl::if_<
        locked,
        boost::mutex,
        boost::interprocess::null_mutex
      >::type mutex_type;
    };


// -------------------------------------------------------------------------- //
// SingletonInitializerBase
// -------------------------------------------------------------------------- //
    template<class T, class Derived, bool isInitializedAtStartup>
    class SingletonInitializerBase {
    public:
      static void instantiateInstance() {
        /* Instantiate sInstance so that it will be initialized at 
         * startup time. */
        use(sInstance);
      }

    private:
      static void use(const T &) {}

      static T &instance() {
        return Derived::instance();
      }

      static T &sInstance;
    };

    template<class T, class Derived, bool isInitializedAtStartup> 
    T &SingletonInitializerBase<T, Derived, isInitializedAtStartup>::sInstance = 
      SingletonInitializerBase<T, Derived, isInitializedAtStartup>::instance();

    template<class T, class Derived>
    class SingletonInitializerBase<T, Derived, false> {
    public:
      static void instantiateInstance() {}
    };


// -------------------------------------------------------------------------- //
// SingletonMutexBase
// -------------------------------------------------------------------------- //
    template<class T, bool isLocked>
    class SingletonMutexBase {
    public:
      static boost::mutex &mutex() {
        static boost::mutex result;

        /* Instantiate sMutex so that it will be initialized at startup time. */
        use(sMutex);

        return result;
      }

    private:
      void use(const boost::mutex &) {}

      static boost::mutex &sMutex;
    };

    template<class T, bool isLocked>
    boost::mutex SingletonMutexBase<T, isLocked>::sMutex = 
      SingletonMutexBase<T, isLocked>::mutex();

    template<class T>
    class SingletonMutexBase<T, false> {
    public:
      static boost::interprocess::null_mutex &mutex() {
        static boost::interprocess::null_mutex result;

        return result;
      }
    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// Singleton
// -------------------------------------------------------------------------- //
  /**
   * Singleton class implements a singleton pattern. That is, for a given
   * template parameter T, only one instance of class T is created.
   */
  template<class T, class PolicyList = default_singleton_policies>
  class Singleton: 
    public boost::noncopyable, 
    public detail::SingletonInitializerBase<T, Singleton<T, PolicyList>, detail::SingletonPolicies<PolicyList>::initialized_at_startup::value>,
    public detail::SingletonMutexBase<T, detail::SingletonPolicies<PolicyList>::locked::value>
  {
  public:
    static T &instance() {
      boost::lock_guard<detail::SingletonPolicies<PolicyList>::mutex_type> lock(mutex());

      T &result = detail::SingletonStorage<T>::staticInstance();

      instantiateInstance();

      return result;
    }

  private:
    static void use(T &) {}

    static T &sInstance;
  };


} // namespace arx

#endif // ARX_SINGLETON_H
