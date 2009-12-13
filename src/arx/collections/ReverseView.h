#ifndef __ARX_COLLECTIONS_REVERSE_VIEW_H__
#define __ARX_COLLECTIONS_REVERSE_VIEW_H__

#include "config.h"
#include <boost/iterator.hpp>
#include "ViewTraits.h"
#include "ViewBase.h"

#include <boost/range/begin.hpp>


namespace arx {
  namespace detail {
// -------------------------------------------------------------------------- //
// 
// -------------------------------------------------------------------------- //
    class ReverseViewTraits {

    };
  } // namespace detail

// -------------------------------------------------------------------------- //
// ReverseView
// -------------------------------------------------------------------------- //
  template<class Base>
  class ReverseView: public ViewBase<Base> {
  public:



  private:
  };

} // namespace arx

#endif // __ARX_COLLECTIONS_REVERSE_VIEW_H__
