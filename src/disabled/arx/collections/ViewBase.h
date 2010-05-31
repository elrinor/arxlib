#ifndef __ARX_COLLECTIONS_VIEW_BASE_H__
#define __ARX_COLLECTIONS_VIEW_BASE_H__

#include "config.h"
#include "ViewTraits.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ViewBase
// -------------------------------------------------------------------------- //
  template<class Base>
  class ViewBase {
  public:
    typedef proxy_tag proxy_category;
    typedef Base base_type;

  protected:
    ViewBase(base_type& base): mBase(base) {}

    typename stored_range<base_type>::type mBase;
  };

}

#endif // __ARX_COLLECTIONS_VIEW_BASE_H__
