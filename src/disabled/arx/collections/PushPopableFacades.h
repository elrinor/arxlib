#ifndef __ARX_COLLECTIONS_PUSH_POPABLE_FACADES_H__
#define __ARX_COLLECTIONS_PUSH_POPABLE_FACADES_H__

#include "config.h"
#include "FacadeBase.h"
#include "RangeFacades.h" /* This one must be registered beforehand. */

namespace arx {
// -------------------------------------------------------------------------- //
// BackPushPopableFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class BackPushPopableFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (value_type));

    void push_back(const value_type& value) {
      this->container().push_back(value);
    }

    void pop_back() {
      this->container().pop_back();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(BackPushPopableFacade, has_push_back<T>)


// -------------------------------------------------------------------------- //
// FrontPushPopableFacade
// -------------------------------------------------------------------------- //
  template<class Base>
  class FrontPushPopableFacade: public Base {
    ARX_INHERITED_FACADE_TPL(Base);
  public:
    ARX_INJECT_TYPES_TPL(Base, (value_type));

    void push_front(const value_type& value) {
      this->container().push_front(value);
    }

    void pop_front() {
      this->container().pop_front();
    }
  };

  ARX_REGISTER_FACADE_IN_ARX_NS(FrontPushPopableFacade, has_push_front<T>)

} // namespace arx

#endif // __ARX_COLLECTIONS_PUSH_POPABLE_FACADES_H__
