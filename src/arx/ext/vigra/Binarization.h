/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#ifndef ARX_EXT_VIGRA_BINARIZATION_H
#define ARX_EXT_VIGRA_BINARIZATION_H

#include "config.h"
#include <cassert>
#include <vector>
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
    static_assert(channels<SrcPixelType>::value == 1, "Can binarize only single-channel images");

    BasicImage<UInt8, SrcAlloc> tmp;
    const BasicImage<UInt8, SrcAlloc>& src = convert_nocopy(givenSrc, tmp);

    /* Skip empty images. */
    if(src.width() <= 0 || src.height() <= 0)
      return;

    /* Create hystogram. */
    std::vector<int> hystogram;
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

      a = static_cast<int>(aNew);
      b = static_cast<int>(bNew);
    }

    /* Binarize. */
    transformImage(srcImageRange(src), destImage(dst, ConvertingAccessor<UInt8, DstPixelType>()), Threshold<UInt8, UInt8>(0, (a + b) / 2, 255, 0));
  }

} // namespace vigra

#endif // ARX_EXT_VIGRA_BINARIZATION_H
