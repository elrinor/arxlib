#ifndef __ARX_EXT_VIGRA_COLORS_H__
#define __ARX_EXT_VIGRA_COLORS_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include "MetaFunctions.h"

namespace vigra {
// -------------------------------------------------------------------------- //
// ColorTraits
// -------------------------------------------------------------------------- //
  template<class PixelType>
  struct ColorTraits {
    static PixelType white() {
      return boost::is_same<typename NumericTraits<PixelType>::isIntegral, VigraTrueType>::value ? 
        PixelType(vigra::NumericTraits<typename channel_type<PixelType>::type>::max()) :
        vigra::NumericTraits<PixelType>::one();
    }

    static PixelType black() {
      return NumericTraits<PixelType>::zero();
    }
  };


// -------------------------------------------------------------------------- //
// white
// -------------------------------------------------------------------------- //
  template<class PixelType>
  PixelType white() {
    return ColorTraits<PixelType>::white();
  }


// -------------------------------------------------------------------------- //
// black
// -------------------------------------------------------------------------- //
  template<class PixelType>
  PixelType black() {
    return ColorTraits<PixelType>::black();
  }

} // namespace vigra

#endif // __ARX_EXT_VIGRA_COLORS_H__
