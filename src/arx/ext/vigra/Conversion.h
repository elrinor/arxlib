#ifndef __ARX_EXT_VIGRA_CONVERSION_H__
#define __ARX_EXT_VIGRA_CONVERSION_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <vigra/rgbvalue.hxx>
#include <vigra/basicimage.hxx>
#include <arx/Utility.h> /* for STATIC_ASSERT() */
#include "MetaFunctions.h"
#include "Accessor.h"

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
        return (static_cast<unsigned long long>(src) + 8421504u) / 16843009u;
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
        return (static_cast<unsigned long long>(src) + 32768u) / 65537u;
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
        return src * 255.0f + 0.5f;
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
        return src * 255.0 + 0.5;
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
        return src * 65535.0f + 0.5f;
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
        return src * 65535.0 + 0.5;
      }
    };

    template<> 
    struct ChannelConverterBase<UInt32, float> {
      float operator() (UInt32 src) const {
        return src / 4294967296.0;
      }
    };

    template<> 
    struct ChannelConverterBase<float, UInt32> {
      UInt32 operator() (float src) const {
        return src * 4294967296.0 + 0.5;
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
        return src * 4294967296.0 + 0.5;
      }
    };
  
  } // namespace detail


// -------------------------------------------------------------------------- //
// ChannelConverter
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class DstPixelType>
  struct ChannelConverter: detail::ChannelConverterBase<SrcPixelType, DstPixelType> {
    STATIC_ASSERT((channels<SrcPixelType>::value == 1 && channels<DstPixelType>::value == 1));

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
        result.setRed(scale(src.red()));
        result.setGreen(scale(src.green()));
        result.setBlue(scale(src.blue()));
        return result;
      }
    };

    template<class SrcChannelType, class DstChannelType>
    struct ConverterBase<TinyVector<SrcChannelType, 4>, TinyVector<DstChannelType, 4> > {
      TinyVector<DstChannelType, 4> convert(const TinyVector<SrcChannelType, 4>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        return TinyVector<DstChannelType, 4>(
          scale(src[0]),
          scale(src[1]),
          scale(src[2]),
          scale(src[3]),
        );
      }
    };

    template<class SrcChannelType, unsigned srcRedIndex, unsigned srcGreenIndex, unsigned srcBlueIndex, class DstChannelType>
    struct ConverterBase<RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>, TinyVector<DstChannelType, 4> > {
      TinyVector<DstChannelType, 4> convert(const RGBValue<SrcChannelType, srcRedIndex, srcGreenIndex, srcBlueIndex>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        return TinyVector<DstChannelType, 4>(
          scale(src.red()),
          scale(src.green()),
          scale(src.blue()),
          scale(white<DstChannelType>()),
        );
      }
    };

    template<class SrcChannelType, class DstChannelType, unsigned dstRedIndex, unsigned dstGreenIndex, unsigned dstBlueIndex>
    struct ConverterBase<TinyVector<SrcChannelType, 4>, RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> > {
      RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> convert(const TinyVector<SrcChannelType, 4>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        RGBValue<DstChannelType, dstRedIndex, dstGreenIndex, dstBlueIndex> result;
        result.setRed(scale(src[0]));
        result.setGreen(scale(src[1]));
        result.setBlue(scale(src[2]));
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
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        return RGBValue<DstChannelType>(scale(src.red()), scale(src.green()), scale(src.blue())).luminance();
      }
    };

    template<class SrcChannelType, class DstChannelType> 
    struct ConverterBase<SrcChannelType, TinyVector<DstChannelType, 4> > {
      TinyVector<DstChannelType, 4> operator() (const SrcChannelType& src) const {
        DstChannelType gray = ChannelConverter<SrcChannelType, DstChannelType>()(src);

        return TinyVector<DstChannelType, 4>(gray, gray, gray, white<DstChannelType>());
      }
    };

    template<class SrcChannelType, class DstChannelType> 
    struct ConverterBase<TinyVector<SrcChannelType, 4>, DstChannelType> {
      DstChannelType operator() (const TinyVector<SrcChannelType, 4>& src) const {
        ChannelConverter<SrcChannelType, DstChannelType> scale;

        return RGBValue<DstChannelType>(scale(src[0]), scale(src[1]), scale(src[2])).luminance();
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

#endif // __ARX_EXT_VIGRA_CONVERSION_H__
