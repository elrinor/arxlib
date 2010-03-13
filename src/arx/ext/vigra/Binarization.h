#ifndef __ARX_EXT_VIGRA_BINARIZATION_H__
#define __ARX_EXT_VIGRA_BINARIZATION_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <vigra/transformimage.hxx> /* for Threshold<> */
#include <arx/Utility.h> /* for STATIC_ASSERT() */
#include "MetaFunctions.h"
#include "Convert.h"

namespace vigra {
// -------------------------------------------------------------------------- //
// kMeansBinarize
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class SrcAlloc, class DstPixelType, class DstAlloc>
  void kMeansBinarize(const BasicImage<SrcPixelType, SrcAlloc>& givenSrc, BasicImage<DstPixelType, DstAlloc>& dst) {
    /* Check channel count. */
    STATIC_ASSERT((channels<SrcPixelType>::value == 1));

    BasicImage<UInt8, SrcAlloc> tmp;
    const BasicImage<UInt8, SrcAlloc>& src = convert_nocopy(givenSrc, tmp);

    /* Skip empty images. */
    if(src.width() <= 0 || src.height() <= 0)
      return;

    /* Create hystogram. */
    arx::vector<int> hystogram;
    hystogram.resize(256, 0);
    for(int y = 0; y < src.height(); y++)
      for(int x = 0; x < src.width(); x++)
        hystogram[src(x, y)]++;

    /* Apply K-means. */
    int a = 0, b = 255;
    while(hystogram[a] == 0)
      a++;
    while(hystogram[b] == 0)
      b--;
    while(true) {
      assert(a <= b);

      long long aNew = 0, bNew = 0;
      int aCount = 0, bCount = 0;
      for(int i = 0; i <= (a + b) / 2; i++) {
        aCount += hystogram[i];
        aNew += static_cast<long long>(i) * hystogram[i];
      }
      for(int i = (a + b) / 2 + 1; i < 256; i++) {
        bCount += hystogram[i];
        bNew += static_cast<long long>(i) * hystogram[i];
      }
      aNew = aCount != 0 ? aNew / aCount : a;
      bNew = bCount != 0 ? bNew / bCount : b;

      if(std::abs(a - static_cast<int>(aNew)) < 1 && std::abs(b - static_cast<int>(bNew)) < 1)
        break; /* We can't use variables from inner scope in while() conditions... */

      a = aNew;
      b = bNew;
    }

    /* Binarize. */
    transformImage(srcImageRange(src), destImage(dst, ConvertingAccessor<UInt8, DstPixelType>()), Threshold<UInt8, UInt8>(0, (a + b) / 2, 255, 0));
  }

} // namespace vigra

#endif // __ARX_EXT_VIGRA_BINARIZATION_H__
