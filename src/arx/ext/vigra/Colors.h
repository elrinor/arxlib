#ifndef __ARX_EXT_VIGRA_COLORS_H__
#define __ARX_EXT_VIGRA_COLORS_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <vigra/rgbvalue.hxx>

namespace vigra {
// -------------------------------------------------------------------------- //
// white
// -------------------------------------------------------------------------- //
  template<class PixelType>
  PixelType white() {
    return boost::is_same<typename NumericTraits<PixelType>::isIntegral, VigraTrueType>::value ? 
      vigra::NumericTraits<PixelType>::max() : 
      vigra::NumericTraits<PixelType>::one();
  }


// -------------------------------------------------------------------------- //
// black
// -------------------------------------------------------------------------- //
  template<class PixelType>
  PixelType black() {
    return NumericTraits<PixelType>::zero();
  }


} // namespace vigra

#endif // __ARX_EXT_VIGRA_COLORS_H__
