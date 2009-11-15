#ifndef __ARX_REVERSEVIEW_H__
#define __ARX_REVERSEVIEW_H__

#include "config.h"
#include <boost/iterator.hpp>
#include "ViewTraits.h"
#include "ViewBase.h"

#include <boost/range/begin.hpp>


namespace arx {
// -------------------------------------------------------------------------- //
// ReverseView
// -------------------------------------------------------------------------- //
  template<class Base>
  class ReverseView: public ViewBase<Base> {
  public:
#define ARX_INJECT(T)                                                           \
    typedef typename base_type::T T
    ARX_INJECT(iterator);
    ARX_INJECT(pointer);
    ARX_INJECT(reference);
    ARX_INJECT(difference_type);
    ARX_INJECT(size_type);
    ARX_INJECT(value_type);
#undef ARX_INJECT



  private:
  };

} // namespace arx

#endif // __ARX_REVERSEVIEW_H__
