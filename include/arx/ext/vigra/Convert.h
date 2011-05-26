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
#ifndef ARX_EXT_VIGRA_CONVERT_H
#define ARX_EXT_VIGRA_CONVERT_H

#include <arx/config.h>
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

#endif // ARX_EXT_VIGRA_CONVERT_H
