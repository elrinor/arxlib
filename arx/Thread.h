#ifndef __ARX_THREAD_H__
#define __ARX_THREAD_H__

#include "config.h"
#include "Utility.h"

#ifdef ARX_USE_BOOST
#  include <boost/thread.hpp>
namespace arx {
  using boost::mutex;
  using boost::thread;
}
#else // ARX_USE_BOOST

namespace arx {
  namespace detail {
    class thread_proc_base {
    public:
      virtual void operator()() = 0;
    };

    template<class F>
    class thread_proc {
    private:
      F f;

    public:
      thread_proc(F f): f(f) {}
      virtual void operator()() {
        f();
      }
    };
  } // namespace detail

  /** Mutex stub - for use as template argument in case thread safety isn't needed. */
  class null_mutex: noncopyable {
  public:
    null_mutex() {}

    static void lock() {}
    static void unlock() {}
  };

  /** Simple lock for the given mutex type. */
  template<typename Mutex>
  class unique_lock {
  private:
    Mutex* mMutex;
    bool mIsLocked;
    unique_lock(unique_lock&);
    unique_lock& operator=(unique_lock&);
  public:
    unique_lock(): mMutex(0), mIsLocked(false) {}

    explicit unique_lock(Mutex& mutex): mMutex(&mutex), mIsLocked(false) {
      lock();
    }

    ~unique_lock() {
      if(owns_lock()) {
        mMutex->unlock();
      }
    }

    void lock() {
      if(owns_lock())
        throw std::runtime_error("unique_lock::lock exception");
      mMutex->lock();
      mIsLocked = true;
    }

    void unlock() {
      if(!owns_lock())
        throw std::runtime_error("unique_lock::unlock exception");
      mMutex->unlock();
      mIsLocked = false;
    }

    typedef void (unique_lock::*bool_type)();

    operator bool_type() const {
      return mIsLocked ? &unique_lock::lock : 0;
    }

    bool operator!() const {
      return !owns_lock();
    }

    bool owns_lock() const {
      return mIsLocked;
    }

    Mutex* mutex() const {
      return mMutex;
    }

    Mutex* release() {
      Mutex* const result = mMutex;
      mMutex = 0;
      mIsLocked = false;
      return result;
    }
  };
}

#ifdef ARX_WIN32
#  define NOMINMAX
#  include <Windows.h>
namespace arx {
  class mutex: noncopyable {
  private:
    CRITICAL_SECTION m;

  public:
    mutex() { InitializeCriticalSection(&m); }
    ~mutex() { DeleteCriticalSection(&m); }

    void lock() { EnterCriticalSection(&m); }
    void unlock() { LeaveCriticalSection(&m); }

    typedef unique_lock<mutex> scoped_lock;
  };

  namespace detail {
    DWORD WINAPI invoke_thread_proc(void* f) {
      /* We're already in another thread... */
      thread_proc_base* threadFunc = reinterpret_cast<thread_proc_base*>(f);
      try {
        threadFunc->operator()();
      catch (...) {}
      delete threadFunc;
    }
  } // namespace detail

  class thread: noncopyable {
  private:
    HANDLE h;
    DWORD id;
    
  public:
    thread() {
      h = INVALID_HANDLE_VALUE;
    }
    
    ~thread() {
      detach();
    }

    template<class F>
    explicit thread(F f): {
      detail::thread_proc_base* threadFunc = new detail::thread_proc<F>(f);
      HANDLE h = CreateThread(0, 0, detail::invoke_thread_proc, reinterpret_cast<void*>(threadFunc), 
        CREATE_SUSPENDED, &this->id);
      if(h == NULL)
        throw std::runtime_error("Failed to create thread.");
      this->h = h;
      ResumeThread(h);
    }

    void detach() {
      this->h = INVALID_HANDLE_VALUE;
    }
      
    typedef HANDLE native_handle_type;
    native_handle_type native_handle() {
      return this->h;
    }
  }
}
#elif defined(_POSIX_THREADS)
#  include <pthread.h>
namespace arx {
  class mutex: noncopyable {
  private:
    pthread_mutex_t m;

  public:
    pthread_mutex() { pthread_mutex_init(&m, 0); }
    ~pthread_mutex() { pthread_mutex_destroy(&m); }

    void lock() { pthread_mutex_lock(&m); }
    void unlock() { pthread_mutex_unlock(&m); }

    typedef unique_lock<mutex> scoped_lock;
  };
}

// TODO thread class

#elif defined(ARX_DISABLE_THREADS)
namespace arx {
  typedef null_mutex mutex;
}
#else
#  error "Threads are not supported on your system, #define ARX_DISABLE_THREADS to turn off multithreading support in ArX Library."
#endif

#endif // ARX_USE_BOOST

namespace arx {
  class thread_pool: public noncopyable {
  public:
    thread_pool(size_t initialThreads) {
      assert(initialThreads > 0);
      mThreadCount = initialThreads;
      mActiveThreadCount = 0;
      mTerminating = false;
      mWakeMutex->lock();
      for(int i = 0; i < initialThreads; i++)
        mWorkerThreads.push_back(new WorkerThread(this));
    }

    ~thread_pool() {
      mMutex.lock();
      mTerminating = true;
      if(mTasks.size() == 0)
        mWakeMutex.unlock();
      mWaitMutex.lock();
      mMutex.unlock();
      mWaitMutex.lock();
      for(int i = 0; i < mWorkerThreads.size(); i++)
        delete mWorkerThreads[i];
      clear();
    }

    size_t size() const {
      return mThreadCount;
    }

    template<class F>
    bool schedule(F f) {
      mutex::scoped_lock(mMutex);
      mTasks.push_back(new detail::thread_proc<F>(f));
      if(mTasks.size() == 1)
        mWakeMutex.unlock();
    }

    size_t active() const {
      return mActiveThreadCount;
    }

    size_t pending() const {
      return mTasks.size();
    }

    void clear() {
      mutex::scoped_lock(mMutex);
      for(int i = 0; i < mTasks.size(); i++)
        delete mTasks[i];
      mTasks.clear();
    }    

    bool empty() const {
      return pending() == 0;
    }   

  private:
    class WorkerThread {
    public:
      WorkerThread(thread_pool* pool): mThread(WorkerProc(pool)) {}

    private:
      thread mThread;
    };

    class WorkerProc {
    public:
      WorkerProc(thread_pool* pool): mPool(pool) {}

      void operator()() {
        detail::thread_proc_base* task = NULL;
        while(true) {
          wakeMutex().lock();

          mutex().lock();
          if(pool().mTerminating) {
            wakeMutex().unlock();
            if(--pool().mThreadCount == 0)
              waitMutex().unlock();
            return;
          } else {
            task = pool().mTasks.front();
            pool().mTasks.erase(pool().mTasks.begin());
            if(!pool().mTasks.empty())
              wakeMutex().unlock();
            pool().mActiveThreadCount++;
          }
          mutex().unlock();

          try {
            task->operator()();
          } catch (...) {}
          delete task;
          task = NULL;

          mutex().lock();
          pool().mActiveThreadCount--;
          mutex().unlock();
        }
      }
    private:
      thread_pool& pool() {
        return *mPool;
      }

      mutex& wakeMutex() {
        return mPool->mWakeMutex;
      }

      mutex& waitMutex() {
        return mPool->mWaitMutex;
      }

      mutex& mutex() {
        return mPool->mMutex;
      }

      thread_pool* mPool;
    };

    volatile std::vector<detail::thread_proc_base*> mTasks;
    std::vector<WorkerThread*> mWorkerThreads;
    volatile size_t mActiveThreadCount;
    size_t mThreadCount;
    mutex mMutex;
    mutex mWakeMutex;
    mutex mWaitMutex;
    bool mTerminating;
  };

} // namespace arx

#endif // __ARX_THREAD_H__
