#ifndef __ARX_SHAREDCONTAINER_H__
#define __ARX_SHAREDCONTAINER_H__

#include "../config.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/detail/atomic_count.hpp>

namespace arx { 
// -------------------------------------------------------------------------- //
// SharedContainer
// -------------------------------------------------------------------------- //
  template<class Base, class Container>
  class SharedContainer: public Base {
  public:
    typedef Container container_type;

  protected:
    friend class Base;
    /* 
    container_type& crtpContainer();
    const container_type& crtpContainer() const;
    */

  private:
    class ContainerPrivate: public Container {
    public:
      ContainerPrivate(): mSharedCount(0) {}
      
      template<class T1>
      ContainerPrivate(const T1& arg1): Container(arg1), mSharedCount(0) {}

      template<class T1, class T2>
      ContainerPrivate(const T1& arg1, const T2& arg2): Container(arg1, arg2), mSharedCount(0) {}

      template<class T1, class T2, class T3>
      ContainerPrivate(const T1& arg1, const T2& arg2): Container(arg1, arg2), mSharedCount(0) {}

    private:
      friend void intrusive_ptr_add_ref(SetPrivate* value) {
        ++value->mSharedCount;
      }

      friend void intrusive_ptr_release(SetPrivate* value) {
        if(--value->mSharedCount == 0)
          delete value;
      }

      friend long intrusive_ptr_use_count(SetPrivate* value) {
        return value->mSharedCount;
      }

      boost::detail::atomic_count mSharedCount;
    };

    boost::shared_ptr<container_type> mContainer;

  };
  
}


#endif // __ARX_SHAREDCONTAINER_H__
