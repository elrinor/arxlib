#ifndef __ARX_ALGORITHM_H__
#define __ARX_ALGORITHM_H__

#include "config.h"
#include <utility>         /* for std::fill, std::equal */
#include <algorithm>       /* for std::find */
#include <boost/range.hpp>

namespace arx {
  template<class Collection, class T>
  void fill(Collection& collection, const T& value) {
    std::fill(boost::begin(collection), boost::end(collection), value);
  }

  template<class LCollection, class RCollection> 
  bool equal(const LCollection& l, const RCollection& r) {
    return std::equal(boost::begin(l), boost::end(l), boost::begin(r));
  }

  template<class Collection, class T>
  boost::range_iterator<Collection> find(Collection& c, const T& value) {
    return std::find(boost::begin(c), boost::end(c), value);
  }

  template<class Collection, class T>
  bool contains(Collection& c, const T& value) {
    return find(c, value) != boost::end(c);
  }

} // namespace arx

#endif // __ARX_ALGORITHM_H__
