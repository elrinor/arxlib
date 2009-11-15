#ifndef __ARX_VIEWBASE_H__
#define __ARX_VIEWBASE_H__

#include "config.h"
#include "ViewTraits.h"

namespace arx {
// -------------------------------------------------------------------------- //
// ViewBase
// -------------------------------------------------------------------------- //
  template<class Base>
  class ViewBase {
  public:
    typedef normal_view_tag view_category;
    typedef Base base_type;

  protected:
    ViewBase(base_type& base): mBase(base) {}

    typename stored_range<base_type>::type mBase;
  };

}

#endif // __ARX_VIEWBASE_H__
