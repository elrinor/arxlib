#ifndef __ARX_COLLECTIONS_ALGORITHM_H__
#define __ARX_COLLECTIONS_ALGORITHM_H__

#include "config.h"
#include <utility>         /* for std::fill, std::equal */
#include <algorithm>       /* for std::find */
#include <boost/range.hpp>

namespace arx {
  template<class ForwardCollection, class T>
  void fill(ForwardCollection& collection, const T& value) {
    std::fill(boost::begin(collection), boost::end(collection), value);
  }

  template<class LInputCollection, class RInputCollection> 
  bool equal(const LInputCollection& l, const RInputCollection& r) {
    return std::equal(boost::begin(l), boost::end(l), boost::begin(r));
  }

  template<class InputCollection, class T>
  typename boost::range_iterator<InputCollection>::type find(InputCollection& c, const T& value) {
    return std::find(boost::begin(c), boost::end(c), value);
  }

  template<class InputCollection, class T>
  bool contains(InputCollection& c, const T& value) {
    return find(c, value) != boost::end(c);
  }

  template<class InputCollection, class OutputCollection>
  typename boost::range_iterator<OutputCollection>::type copy(const InputCollection& from, OutputCollection& to) {
    return std::copy(boost::begin(from), boost::end(from), boost::begin(to));
  }

  template<class InputCollection, class InsertableCollection>
  void back_insert(const InputCollection& from, InsertableCollection& to) {
    std::copy(boost::begin(from), boost::end(from), std::inserter(to, boost::end(to)));
  }


} // namespace arx

#endif // __ARX_COLLECTIONS_ALGORITHM_H__
