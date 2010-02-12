#ifndef __ARX_LIB_VIGRA_H__
#define __ARX_LIB_VIGRA_H__

#include "config.h"

#include <vigra/basicimage.hxx>
#include <vigra/stdimage.hxx>
#include <vigra/impex.hxx>

namespace vigra {
  typedef BasicImage<TinyVector<UInt8, 4> > BRGBAImage;

// -------------------------------------------------------------------------- //
// RGBAToGrayAccessor
// -------------------------------------------------------------------------- //
  template<class RGBAValue>
  class RGBAToGrayAccessor {
  public:
    typedef typename RGBAValue::value_type value_type;

    template<class Iterator>
    value_type operator()(Iterator const & i) const {
      return RGBValue<value_type>((*i)[0], (*i)[1], (*i)[2]).luminance();
    }

    template <class Iterator, class Difference>
    value_type operator()(Iterator const & i, Difference d) const {
      return operator() (i + d);
    }
  };


// -------------------------------------------------------------------------- //
// RGBAToRGBAccessor
// -------------------------------------------------------------------------- //
  template <class RGBAValue>
  class RGBAToRGBAccessor {
  public:
    typedef RGBValue<typename RGBAValue::value_type> value_type;

    template<class Iterator>
    value_type operator()(Iterator const & i) const {
      return value_type((*i)[0], (*i)[1], (*i)[2]);
    }

    template <class Iterator, class Difference>
    value_type operator()(Iterator const & i, Difference d) const {
      return operator() (i + d);
    }
  };

  template<class PixelType, class Alloc, class Color>
  inline void checkedSetPixel(BasicImage<PixelType, Alloc>& image, int x, int y, const Color& value) {
    typename BasicImage<PixelType, Alloc>::difference_type coord(x, y);
    if(image.isInside(coord))
      image[coord] = value;
  }

  template<class PixelType, class Alloc>
  inline void importGrayscaleImage(BasicImage<PixelType, Alloc>& out, const std::string& fileName) {
    ImageImportInfo info(fileName.c_str());
    out.resize(info.width(), info.height());
    if(info.isGrayscale()) {
      importImage(info, destImage(out));
    } else if(info.numBands() == 4) {
      BRGBAImage rgbaImage(info.width(), info.height());
      importImage(info, destImage(rgbaImage));
      copyImage(srcImageRange(rgbaImage, RGBAToGrayAccessor<BRGBAImage::value_type>()), destImage(out));
    } else {
      BRGBImage rgbImage(info.width(), info.height());
      importImage(info, destImage(rgbImage));
      copyImage(srcImageRange(rgbImage, RGBToGrayAccessor<BRGBImage::value_type>()), destImage(out));
    }
  }

  template<class PixelType, class Alloc>
  inline void importRgbImage(BasicImage<PixelType, Alloc>& out, const std::string& fileName) {
    ImageImportInfo info(fileName.c_str());
    out.resize(info.width(), info.height());
    if(info.isGrayscale()) {
      BImage bImage(info.width(), info.height());
      importImage(info, destImage(bImage));
      copyImage(srcImageRange(bImage, GrayToRGBAccessor<BImage::value_type>()), destImage(out));
    } else if(info.numBands() == 4) {
      BRGBAImage rgbaImage(info.width(), info.height());
      importImage(info, destImage(rgbaImage));
      copyImage(srcImageRange(rgbaImage, RGBAToRGBAccessor<BRGBAImage::value_type>()), destImage(out));
    } else {
      importImage(info, destImage(out));
    }
  }

  template<class PixelType, class Alloc, class Color>
  inline void drawLine(BasicImage<PixelType, Alloc>& image, int x1, int y1, int x2, int y2, const Color& value) {
    int dx, dy;

    if(x1 == x2 && y1 == y2) {
      checkedSetPixel(image, x1, y1, value);
    } else if(abs(x2 - x1) > abs(y2 - y1)) {
      if(x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }
      dx = x2 - x1;
      dy = y2 - y1;
      for(int i = x1; i <= x2; i++)
        checkedSetPixel(image, i, y1 + (i - x1) * dy / dx, value);
    } else {
      if(y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }
      dx = x2 - x1;
      dy = y2 - y1;
      for(int i = y1; i <= y2; i++)
        checkedSetPixel(image, x1 + (i - y1) * dx / dy, i, value);
    }
  }

} // namespace vigra

#endif // __ARX_LIB_VIGRA_H__
