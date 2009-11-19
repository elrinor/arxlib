#ifndef __ARX_COLLECTIONS_SHARED_H__
#define __ARX_COLLECTIONS_SHARED_H__

#include "../config.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/detail/atomic_count.hpp>
#include "Forward.h"

namespace arx { 
// -------------------------------------------------------------------------- //
// Shared
// -------------------------------------------------------------------------- //
  template<class T>
  class Shared {
  public:
    typedef T element_type;

    ARX_FORWARD_CONSTRUCTOR(Shared, (ARX_FWD_TEXT(mPrivate))(ARX_FWD_LPAREN())(ARX_FWD_TEXT(new SharedPrivate))(ARX_FWD_ARGS())(ARX_FWD_RPAREN()), ({}));

    element_type* operator-> () const {
      return &mPrivate->mValue;
    }

    element_type& operator* () const {
      return mPrivate->mValue;
    }

    bool unique() const {
      return intrusive_ptr_use_count(mPrivate.get()) == 1;
    }

    void detach() {
      if(!unique())
        mPrivate.reset(new SharedPrivate(*mPrivate));
    }

  private:
    class SharedPrivate {
    public:
      ARX_FORWARD_CONSTRUCTOR(SharedPrivate, (ARX_FWD_TEXT(mValue))(ARX_FWD_ARGS())(ARX_FWD_COMMA())(ARX_FWD_TEXT(mSharedCount(0))), ({}))

    private:
      friend void intrusive_ptr_add_ref(SharedPrivate* value) {
        ++value->mSharedCount;
      }

      friend void intrusive_ptr_release(SharedPrivate* value) {
        if(--value->mSharedCount == 0)
          delete value;
      }

      friend long intrusive_ptr_use_count(SharedPrivate* value) {
        return value->mSharedCount;
      }

      element_type mValue;
      boost::detail::atomic_count mSharedCount;
    };

    boost::intrusive_ptr<SharedPrivate> mPrivate;
  };
  
}


#endif // __ARX_COLLECTIONS_SHARED_H__
