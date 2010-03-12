#ifndef __ARX_EXT_VIGRA_DRAWING_H__
#define __ARX_EXT_VIGRA_DRAWING_H__

#include "config.h"
#include <algorithm> /* for std::swap() */

namespace vigra {
  namespace detail {
// -------------------------------------------------------------------------- //
// Line drawing
// -------------------------------------------------------------------------- //
    template<class PixelType, class Alloc>
    inline void checkedSetPixel(BasicImage<PixelType, Alloc>& image, int x, int y, const PixelType& value) {
      typename BasicImage<PixelType, Alloc>::difference_type coord(x, y);
      if(image.isInside(coord))
        image[coord] = value;
    }
  } // namespace detail

  template<class PixelType, class Alloc>
  inline void drawLine(BasicImage<PixelType, Alloc>& image, int x1, int y1, int x2, int y2, const PixelType& value) {
    int dx, dy;

    if(x1 == x2 && y1 == y2) {
      detail::checkedSetPixel(image, x1, y1, value);
    } else if(abs(x2 - x1) > abs(y2 - y1)) {
      if(x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }
      dx = x2 - x1;
      dy = y2 - y1;
      for(int i = x1; i <= x2; i++)
        detail::checkedSetPixel(image, i, y1 + (i - x1) * dy / dx, value);
    } else {
      if(y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
      }
      dx = x2 - x1;
      dy = y2 - y1;
      for(int i = y1; i <= y2; i++)
        detail::checkedSetPixel(image, x1 + (i - y1) * dx / dy, i, value);
    }
  }

} // namespace vigra

#endif // __ARX_EXT_VIGRA_DRAWING_H__
