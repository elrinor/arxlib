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
#ifndef ARX_EXT_VIGRA_VALIDATION_H
#define ARX_EXT_VIGRA_VALIDATION_H

#include "config.h"
#include <boost/program_options.hpp>
#include <boost/spirit/include/qi.hpp>

namespace vigra {
// -------------------------------------------------------------------------- //
// Validators
// -------------------------------------------------------------------------- //
  /** Validation function for vigra::Size2D command line parameter. 
   * Expects the size to be in the format w:h. */
  inline void validate(boost::any& v, const std::vector<std::string>& values, Size2D* /* target_type */, int) {
    using namespace boost::program_options;
    using namespace boost::spirit::qi;

    /* Check that it's the only one. */
    validators::check_first_occurrence(v);
    const std::string& s = validators::get_single_string(values);

    /* Match. */
    int w, h;
    std::string::const_iterator begin = s.begin();
    if(!parse(begin, s.end(), uint_ >> ':' >> uint_, w, h))
      throw invalid_option_value(s);

    /* Ok. */
    v = Size2D(w, h);
  }

  /** Validation function for vigra::Rect2D command line parameter. 
   * Expects the rectangle to be in the format x:y:w:h. Negative or zero 
   * width and height must be additionally processed with fixNegativeSize function. */
  inline void validate(boost::any& v, const std::vector<std::string>& values, Rect2D* /* target_type */, int) {
    using namespace boost::program_options;
    using namespace boost::spirit::qi;

    /* Check that it's the only one. */
    validators::check_first_occurrence(v);
    const std::string& s = validators::get_single_string(values);

    /* Match. */
    int x, y, w, h;
    std::string::const_iterator begin = s.begin();
    if(!parse(begin, s.end(), uint_ >> ':' >> uint_ >> ':' >> int_ >> ':' >> int_, x, y, w, h))
      throw invalid_option_value(s);

    /* Ok. */
    v = Rect2D(x, y, x + w, y + h);
  }


// -------------------------------------------------------------------------- //
// Size adjustment
// -------------------------------------------------------------------------- //
  /** Adjusts the negative or zero width and height of the input rectangle. */
  inline void fixNegativeSize(Rect2D* rect, const Size2D& size) {
    int newWidth = rect->width();
    if(newWidth <= 0)
      newWidth = size.width() - rect->left() + newWidth;

    int newHeight = rect->height();
    if(newHeight <= 0)
      newHeight = size.height() - rect->top() + newHeight;

    rect->setSize(newWidth, newHeight);
  }

  inline void fixNegativeSize(Rect2D* rect, int width, int height) {
    fixNegativeSize(rect, Size2D(width, height));
  }

} // namespace vigra

#endif // ARX_EXT_VIGRA_VALIDATION_H
