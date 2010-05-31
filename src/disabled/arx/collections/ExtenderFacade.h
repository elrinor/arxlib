#ifndef __ARX_COLLECTIONS_EXTENDER_FACADE_H__
#define __ARX_COLLECTIONS_EXTENDER_FACADE_H__

#include "config.h"
#include "Extender.h"
#include "Facade.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ExtenderFacade
// -------------------------------------------------------------------------- //
  template<class T>
  class ExtenderFacade: public Extender<Facade<T, ExtenderFacade<T> >, ExtenderFacade<T> > {
  public:
    ExtenderFacade(T& value): mPrivate(value) {}

  private:
    T& container() {
      return mPrivate;
    }

    const T& container() const {
      return mPrivate;
    }

    friend FacadeBase<T, ExtenderFacade>;

    T& mPrivate; /* TODO: C++0x rvalues need by-value storage. */
  };

}


#endif // __ARX_COLLECTIONS_EXTENDER_FACADE_H__
