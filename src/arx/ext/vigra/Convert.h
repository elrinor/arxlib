#ifndef __ARX_EXT_VIGRA_CONVERT_H__
#define __ARX_EXT_VIGRA_CONVERT_H__

#include "config.h"
#include "Converter.h"
#include "Accessors.h"

namespace vigra {
// -------------------------------------------------------------------------- //
// convert
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class SrcAlloc, class DstPixelType, class DstAlloc>
  void convert(const BasicImage<SrcPixelType, SrcAlloc>& srcImage, BasicImage<DstPixelType, DstAlloc>& dstImage) {
    if(srcImage.width() == 0 && srcImage.height() == 0) {
      dstImage = BasicImage<DstPixelType, DstAlloc>();
    } else {
      dstImage.resize(srcImage.size());

      copyImage(srcImageRange(srcImage, ConvertingAccessor<SrcPixelType, DstPixelType>()), destImage(dstImage));
    }
  }


// -------------------------------------------------------------------------- //
// convert_nocopy
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class SrcAlloc, class DstPixelType, class DstAlloc>
  const BasicImage<DstPixelType, DstAlloc>& convert_nocopy(const BasicImage<SrcPixelType, SrcAlloc>& srcImage, BasicImage<DstPixelType, DstAlloc>& dstImage) {
    if(boost::is_same<SrcPixelType, DstPixelType>::value && boost::is_same<SrcAlloc, DstAlloc>::value) {
      return reinterpret_cast<const BasicImage<DstPixelType, DstAlloc>&>(srcImage);
    } else {
      convert(srcImage, dstImage);
      return dstImage;
    }
  }

} // namespace vigra

#endif // __ARX_EXT_VIGRA_CONVERT_H__
