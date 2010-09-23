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
#ifndef ARX_EXT_VIGRA_DRAWING_H
#define ARX_EXT_VIGRA_DRAWING_H

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

#endif // ARX_EXT_VIGRA_DRAWING_H
