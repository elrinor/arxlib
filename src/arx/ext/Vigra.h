#ifndef __ARX_EXT_VIGRA_H__
#define __ARX_EXT_VIGRA_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <vigra/basicimage.hxx>
#include <vigra/stdimage.hxx>

namespace vigra {
  typedef BasicImage<TinyVector<UInt8, 4> > BRGBAImage;

  namespace detail {
// -------------------------------------------------------------------------- //
// ToRGBAAccessorBase
// -------------------------------------------------------------------------- //
    class ToRGBAAccessorBase {
    protected:
      template<class Value>
      Value opaque() const {
        return boost::is_same<NumericTraits<Value>::isIntegral, VigraTrueType>::value ? 
          vigra::NumericTraits<Value>::max() : 
          vigra::NumericTraits<Value>::one();
      }
    };

  } // namespace detail

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
// GrayToRGBAAccessor
// -------------------------------------------------------------------------- //
  template<class Value>
  class GrayToRGBAAccessor: private detail::ToRGBAAccessorBase {
  public:
    typedef TinyVector<Value, 4> value_type;

    template<class Iterator>
    value_type operator()(Iterator const & i) const {
      return value_type(*i, *i, *i, opaque<Value>());
    }

    template <class Iterator, class Difference>
    value_type operator()(Iterator const & i, Difference d) const {
      return operator() (i + d);
    }
  };


// -------------------------------------------------------------------------- //
// RGBAToRGBAccessor
// -------------------------------------------------------------------------- //
  template<class RGBAValue>
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


// -------------------------------------------------------------------------- //
// RGBAToRGBAccessor
// -------------------------------------------------------------------------- //
  template<class RGBValue>
  class RGBToRGBAAccessor: private detail::ToRGBAAccessorBase {
  public:
    typedef TinyVector<typename RGBValue::value_type, 4> value_type;

    template<class Iterator>
    value_type operator()(Iterator const & i) const {
      return value_type((*i)[0], (*i)[1], (*i)[2], opaque<typename RGBValue::value_type>());
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


// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_VIGRA
#  define ARX_USE_VIGRA
#endif

#ifdef ARX_USE_MAGICK
#  include "VigraMagick.h"
#endif

#endif // __ARX_EXT_VIGRA_H__
