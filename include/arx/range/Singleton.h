/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2011 Alexander Fokin <apfokin@gmail.com>
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
#ifndef ARX_RANGE_SINGLETON_H
#define ARX_RANGE_SINGLETON_H

#include "config.h"
#include <boost/type_traits/remove_reference.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace arx {
  template<class T>
  class singleton_range;

  namespace range_detail {
    template<class ValueType>
    class singleton_range_iterator_base: public boost::iterator_facade<
      singleton_range_iterator_base<ValueType>,
      ValueType,
      boost::bidirectional_traversal_tag
    > {
    protected:
      friend class boost::iterator_core_access;

      singleton_range_iterator_base(): mValue(NULL) {}

      singleton_range_iterator_base(ValueType *valuePointer): mValuePointer(valuePointer) {}

      ValueType &dereference() const {
        return *mValuePointer;
      }
      
      template<class OtherValueType>
      bool equal(const singleton_range_iterator_base<OtherValueType> &other) const {
        return mValuePointer == other.mValuePointer;
      }

      void increment() { 
        mValuePointer++;
      }

      void decrement() { 
        mValuePointer--;
      }

      ValueType *mValuePointer;
    };

    template<class ValueType>
    class singleton_range_iterator;

    template<class ValueType>
    class const_singleton_range_iterator: public singleton_range_iterator_base<const ValueType> {
      typedef singleton_range_iterator_base<const ValueType> base_type;
    
    public:
      const_singleton_range_iterator() {}

    protected:
      const_singleton_range_iterator(const ValueType *valuePointer): base_type(valuePointer) {}

      friend class singleton_range_iterator<ValueType>;

      template<class T> friend class singleton_range;
    };

    template<class ValueType>
    class singleton_range_iterator: public singleton_range_iterator_base<ValueType> {
      typedef singleton_range_iterator_base<ValueType> base_type;

    public:
      singleton_range_iterator() {}

      singleton_range_iterator(const const_singleton_range_iterator<ValueType> &other): base_type(other.mValuePointer) {}

    protected:
      singleton_range_iterator(ValueType *valuePointer): base_type(valuePointer) {}

      template<class T> friend class singleton_range;
    };


  } // namespace range_detail


// -------------------------------------------------------------------------- //
// singleton_range
// -------------------------------------------------------------------------- //
  template<class T>
  class singleton_range {
  public:
    typedef T stored_type;
    typedef typename boost::remove_reference<stored_type>::type value_type;

    typedef range_detail::singleton_range_iterator<value_type> iterator;
    typedef range_detail::const_singleton_range_iterator<value_type> const_iterator;

    singleton_range(value_type &value): mValue(value) {}

    singleton_range(const singleton_range &other): mValue(other.mValue) {}

    iterator begin() {
      return iterator(&mValue);
    }

    iterator end() {
      return iterator(&mValue + 1);
    }

    const_iterator begin() const {
      return const_iterator(&mValue);
    }

    const_iterator end() const {
      return const_iterator(&mValue + 1);
    }

  private:
    stored_type mValue;
  };

} // namespace arx

#endif // ARX_RANGE_SINGLETON_H
