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
#ifndef ARX_EXT_QT_RANGE_H
#define ARX_EXT_QT_RANGE_H

#include "config.h"
#include <utility> /* For std::pair. */
#include <boost/iterator/iterator_adaptor.hpp>
#include <QHash>
#include <QMultiHash>
#include <QMap>
#include <QMultiMap>

/*
 * Qt associative containers are not compatible with stl associative containers
 * as they provide iterators over a set of mapped values, not over a set of 
 * key-value pairs.
 *
 * This problem is solved by introducing proper boost::range bindings for 
 * these containers.
 */

namespace arx { namespace detail {
// -------------------------------------------------------------------------- //
// QIteratorWrapper
// -------------------------------------------------------------------------- //
  template<class Iterator, class Value, class Reference>
  class QIteratorWrapper: public 
    boost::iterator_adaptor<
      QIteratorWrapper<Iterator, Value, Reference>, 
      Iterator,
      Value,
      boost::bidirectional_traversal_tag,
      Reference
    >
  {
  public:
    QIteratorWrapper() {}

    explicit QIteratorWrapper(const Iterator &iterator): 
      QIteratorWrapper::iterator_adaptor_(iterator) {}

    template<class OtherIterator, class OtherKey, class OtherT>
    QIteratorWrapper(const QIteratorWrapper<OtherIterator, OtherKey, OtherT> &other): 
      QIteratorWrapper::iterator_adaptor_(other.base()) {}

  private:
    friend class boost::iterator_core_access;

    Reference dereference() const { 
      return Reference(QIteratorWrapper::base().key(), QIteratorWrapper::base().value());
    }
  };

}} // namespace arx::detail

#define ARX_REGISTER_QT_ITERATOR_WRAPPER(CONTAINER)                             \
namespace boost {                                                               \
                                                                                \
  template<class Key, class T>                                                  \
  struct range_mutable_iterator<CONTAINER<Key, T> > {                           \
    typedef arx::detail::QIteratorWrapper<typename CONTAINER<Key, T>::iterator, std::pair<const Key, T>, std::pair<const Key &, T &> > type; \
  };                                                                            \
                                                                                \
  template<class Key, class T>                                                  \
  struct range_const_iterator<CONTAINER<Key, T> > {                             \
    typedef arx::detail::QIteratorWrapper<typename CONTAINER<Key, T>::const_iterator, std::pair<const Key, T>, std::pair<const Key &, const T &> > type; \
  };                                                                            \
                                                                                \
}                                                                               \
                                                                                \
template<class Key, class T>                                                    \
inline typename boost::range_mutable_iterator<CONTAINER<Key, T> >::type         \
range_begin(CONTAINER<Key, T> &x) {                                             \
  return typename boost::range_mutable_iterator<CONTAINER<Key, T> >::type(x.begin()); \
}                                                                               \
                                                                                \
template<class Key, class T>                                                    \
inline typename boost::range_const_iterator<CONTAINER<Key, T> >::type           \
range_begin(const CONTAINER<Key, T> &x) {                                       \
  return typename boost::range_const_iterator<CONTAINER<Key, T> >::type(x.begin()); \
}                                                                               \
                                                                                \
template<class Key, class T>                                                    \
inline typename boost::range_mutable_iterator<CONTAINER<Key, T> >::type         \
range_end(CONTAINER<Key, T> &x) {                                               \
  return typename boost::range_mutable_iterator<CONTAINER<Key, T> >::type(x.end()); \
}                                                                               \
                                                                                \
template<class Key, class T>                                                    \
inline typename boost::range_const_iterator<CONTAINER<Key, T> >::type           \
range_end(const CONTAINER<Key, T> &x) {                                         \
  return typename boost::range_const_iterator<CONTAINER<Key, T> >::type(x.end()); \
}

ARX_REGISTER_QT_ITERATOR_WRAPPER(QHash);
ARX_REGISTER_QT_ITERATOR_WRAPPER(QMultiHash);
ARX_REGISTER_QT_ITERATOR_WRAPPER(QMap);
ARX_REGISTER_QT_ITERATOR_WRAPPER(QMultiMap);

#undef ARX_REGISTER_QT_ITERATOR_WRAPPER

#endif // ARX_EXT_QT_RANGE_H
