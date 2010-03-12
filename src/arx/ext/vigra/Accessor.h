#ifndef __ARX_EXT_VIGRA_ACCESSOR_H__
#define __ARX_EXT_VIGRA_ACCESSOR_H__

#include "config.h"
#include <boost/type_traits/is_same.hpp>
#include <arx/Utility.h> /* for STATIC_ASSERT() */
#include "MetaFunctions.h"
#include "Conversion.h"

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

#endif // __ARX_EXT_VIGRA_ACCESSOR_H__
