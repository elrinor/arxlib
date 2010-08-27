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
#ifndef ARX_EXT_VIGRA_ACCESSORS_H
#define ARX_EXT_VIGRA_ACCESSORS_H

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <arx/Utility.h> /* for STATIC_ASSERT() */
#include "MetaFunctions.h"
#include "Converter.h"

namespace vigra {
// -------------------------------------------------------------------------- //
// ConvertingAccessor
// -------------------------------------------------------------------------- //
  template<class SrcPixelType, class DstPixelType>
  struct ConvertingAccessor {
    typedef DstPixelType value_type;

    template<class Iterator>
    value_type operator()(const Iterator& i) const {
      return mConverter(*i);
    }

    template<class Iterator, class Difference>
    value_type operator()(const Iterator& i, const Difference& d) const {
      return operator() (i + d);
    }

    template<class Iterator>
    void set(const SrcPixelType& value, const Iterator& i) const {
      *i = mConverter(value);
    }

    template<class Iterator, class Difference>
    void set(const SrcPixelType& value, const Iterator& i, const Difference& d) const {
      set(value, i + d);
    }

  private:
    Converter<SrcPixelType, DstPixelType> mConverter;
  };


// -------------------------------------------------------------------------- //
// RGBAToGrayAccessor
// -------------------------------------------------------------------------- //
  template<class RGBAValue>
  struct RGBAToGrayAccessor: ConvertingAccessor<RGBAValue, typename RGBAValue::value_type> {};


// -------------------------------------------------------------------------- //
// GrayToRGBAAccessor
// -------------------------------------------------------------------------- //
  template<class Value>
  struct GrayToRGBAAccessor: ConvertingAccessor<Value, TinyVector<Value, 4> > {};


// -------------------------------------------------------------------------- //
// RGBAToRGBAccessor
// -------------------------------------------------------------------------- //
  template<class RGBAValue>
  struct RGBAToRGBAccessor: ConvertingAccessor<RGBAValue, RGBValue<typename RGBAValue::value_type> > {};


// -------------------------------------------------------------------------- //
// RGBToRGBAAccessor
// -------------------------------------------------------------------------- //
  template<class RGBValue>
  struct RGBToRGBAAccessor: ConvertingAccessor<RGBValue, TinyVector<typename RGBValue::value_type, 4> > {};


} // namespace vigra

#endif // ARX_EXT_VIGRA_ACCESSORS_H
