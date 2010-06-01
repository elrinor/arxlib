#ifndef ARX_RANGE_H
#define ARX_RANGE_H

#include "config.h"
#include <algorithm>
#include <boost/utility/enable_if.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include "HasXxx.h"

namespace arx {
  namespace range_protected {
    ARX_HAS_FUNC_XXX_TRAIT_DEF(find);

    template<class AssociativeContainer, class T, bool hasFind = has_find<AssociativeContainer>::value>
    struct Contains {
      bool operator() (const AssociativeContainer &container, const T &value) {
        return container.find(value) != container.end();
      }
    };

    template<class Range, class T>
    struct Contains<Range, T, false> {
      bool operator() (const Range &range, const T &value) {
        return std::find(boost::begin(range), boost::end(range), value) != boost::end(range);
      }
    };

  } // namespace range_protected

  template<class Range, class T>
  bool contains(const Range &range, const T &value) {
    /* TODO: range type & concept check */

    return range_protected::Contains<Range, T>()(range, value);
  }

} // namespace arx

#endif // ARX_RANGE_H
