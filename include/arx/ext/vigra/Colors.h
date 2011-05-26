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
#ifndef ARX_EXT_VIGRA_COLORS_H
#define ARX_EXT_VIGRA_COLORS_H

#include <arx/config.h>
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

#endif // ARX_EXT_VIGRA_COLORS_H
