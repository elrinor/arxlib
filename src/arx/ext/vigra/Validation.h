#ifndef __ARX_EXT_VIGRA_VALIDATION_H__
#define __ARX_EXT_VIGRA_VALIDATION_H__

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
    if(!parse(s.begin(), s.end(), uint_ >> ':' >> uint_, w, h))
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
    if(!parse(s.begin(), s.end(), uint_ >> ':' >> uint_ >> ':' >> int_ >> ':' >> int_, x, y, w, h))
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

#endif // __ARX_EXT_VIGRA_VALIDATION_H__
