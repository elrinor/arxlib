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
#ifndef ARX_EXT_VIGRA_CONVERTER_H
#define ARX_EXT_VIGRA_CONVERTER_H

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <vigra/rgbvalue.hxx>
#include <vigra/basicimage.hxx>
#include <arx/Utility.h> /* for STATIC_ASSERT() */
#include "MetaFunctions.h"
#include "RGBAValue.h"
#include "Colors.h"
#include "Accessors.h"

namespace vigra {
  namespace detail {
// -------------------------------------------------------------------------- //
// ChannelConverterBase
// -------------------------------------------------------------------------- //
    template<class SrcPixelType, class DstPixelType>
    struct ChannelConverterBase {
      DstPixelType operator() (const SrcPixelType& src) const {
        return src;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt8, UInt16> {
      UInt16 operator() (UInt8 src) const {
        return src * 257u;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt16, UInt8> {
      UInt8 operator() (UInt16 src) const {
        return (src + 128u) / 257u;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt8, UInt32> {
      UInt32 operator() (UInt8 src) const {
        return src * 16843009u;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt32, UInt8> {
      UInt8 operator() (UInt32 src) const {
        return static_cast<UInt8>((static_cast<unsigned long long>(src) + 8421504u) / 16843009u);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt16, UInt32> {
      UInt32 operator() (UInt16 src) const {
        return src * 65537u;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt32, UInt16> {
      UInt16 operator() (UInt32 src) const {
        return static_cast<UInt16>((static_cast<unsigned long long>(src) + 32768u) / 65537u);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt8, float> {
      float operator() (UInt8 src) const {
        return src / 255.0f;
      }
    };

    template<> 
    struct ChannelConverterBase<float, UInt8> {
      UInt8 operator() (float src) const {
        return static_cast<UInt8>(src * 255.0f + 0.5f);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt8, double> {
      double operator() (UInt8 src) const {
        return src / 255.0;
      }
    };

    template<> 
    struct ChannelConverterBase<double, UInt8> {
      UInt8 operator() (double src) const {
        return static_cast<UInt8>(src * 255.0 + 0.5);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt16, float> {
      float operator() (UInt16 src) const {
        return src / 65535.0f;
      }
    };

    template<> 
    struct ChannelConverterBase<float, UInt16> {
      UInt16 operator() (float src) const {
        return static_cast<UInt16>(src * 65535.0f + 0.5f);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt16, double> {
      double operator() (UInt16 src) const {
        return src / 65535.0;
      }
    };

    template<> 
    struct ChannelConverterBase<double, UInt16> {
      UInt16 operator() (double src) const {
        return static_cast<UInt16>(src * 65535.0 + 0.5);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt32, float> {
      float operator() (UInt32 src) const {
        return static_cast<float>(src / 4294967296.0);
      }
    };

    template<> 
    struct ChannelConverterBase<float, UInt32> {
      UInt32 operator() (float src) const {
        return static_cast<UInt32>(src * 4294967296.0 + 0.5);
      }
    };

    template<> 
    struct ChannelConverterBase<UInt32, double> {
      double operator() (UInt32 src) const {
        return src / 4294967296.0;
      }
    };

    template<> 
    struct ChannelConverterBase<double, UInt32> {
      UInt32 operator() (double src) const {
        return static_cast<UInt32>(src * 4294967296.0 + 0.5);
      }
    };
  
  } // namespace detail


// -------------------------------------------------------------------------- //
// ChannelConverter
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class DstPixelType>
  struct ChannelConverter: detail::ChannelConverterBase<SrcPixelType, DstPixelType> {
    static_assert(channels<SrcPixelType>::value == 1 && channels<DstPixelType>::value == 1, "Source and target pixel types must have one channel.");

    typedef DstPixelType result_type;
  };


  namespace detail {
// -------------------------------------------------------------------------- //
// ConverterBase
// -------------------------------------------------------------------------- //
    template<class SrcPixelType, class DstPixelType>
    struct ConverterBase {
      DstPixelType operator() (const SrcPixelType& src) const {
        return ChannelConverter<SrcPixelType, DstPixelType>()(src);
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex> 
    struct ConverterBase<RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>, RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> > {
      RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> operator() (const RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> result;
        result.setRGB(scale(src.red()), scale(src.green()), scale(src.blue()));
        return result;
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, unsigned srcAlphaIndex, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex, unsigned dstAlphaIndex>
    struct ConverterBase<RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>, RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> > {
      RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> operator() (const RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> result;
        result.setRGBA(scale(src.red()), scale(src.green()), scale(src.blue()), scale(src.alpha()));
        return result;
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex, unsigned dstAlphaIndex>
    struct ConverterBase<RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>, RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> > {
      RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> operator() (const RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> result;
        result.setRGBA(scale(src.red()), scale(src.green()), scale(src.blue()), white<DstChannelType>());
        return result;
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, unsigned srcAlphaIndex, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex>
    struct ConverterBase<RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>, RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> > {
      RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> operator() (const RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> result;
        result.setRGB(scale(src.red()), scale(src.green()), scale(src.blue()));
        return result;
      }
    };

    template<class SrcChannelType, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex> 
    struct ConverterBase<SrcChannelType, RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> > {
      RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> operator() (const SrcChannelType& src) const {
        DstChannelType gray = ChannelConverter<SrcChannelType, DstChannelType>()(src);

        return RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex>(gray, gray, gray);
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, class DstChannelType> 
    struct ConverterBase<RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>, DstChannelType> {
      DstChannelType operator() (const RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>& src) const {
        return ChannelConverter<SrcChannelType, DstChannelType>()(src.luminance());
      }
    };

    template<class SrcChannelType, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex, unsigned dstAlphaIndex> 
    struct ConverterBase<SrcChannelType, RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> > {
      RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> operator() (const SrcChannelType& src) const {
        DstChannelType gray = ChannelConverter<SrcChannelType, DstChannelType>()(src);

        RGBAValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex, dstAlphaIndex> result;
        result.setRGBA(gray, gray, gray, white<DstChannelType>());
        return result;
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, unsigned srcAlphaIndex, class DstChannelType> 
    struct ConverterBase<RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>, DstChannelType> {
      DstChannelType operator() (const RGBAValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex, srcAlphaIndex>& src) const {
        return ChannelConverter<SrcChannelType, DstChannelType>()(src.luminance());
      }
    };

  } // namespace detail


// -------------------------------------------------------------------------- //
// Converter
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class DstPixelType>
  struct Converter: detail::ConverterBase<SrcPixelType, DstPixelType> {
    typedef DstPixelType result_type;
  };

} // namespace vigra

#endif // ARX_EXT_VIGRA_CONVERTER_H
