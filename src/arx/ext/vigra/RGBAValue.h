#ifndef __ARX_EXT_VIGRA_RGBA_VALUE_H__
#define __ARX_EXT_VIGRA_RGBA_VALUE_H__

#include "config.h"
#include <vigra/metaprogramming.hxx>
#include <vigra/static_assert.hxx>
#include <vigra/tinyvector.hxx>

#include <vigra/rgbvalue.hxx>

namespace vigra {
  namespace detail {
    template<unsigned I, unsigned R, unsigned G, unsigned B, unsigned A>
    struct SelectColorIndexRHS4;

    template <unsigned R, unsigned G, unsigned B, unsigned A>
    struct SelectColorIndexRHS4<0, R, G, B, A> {
      enum { res = R };
    };

    template <unsigned R, unsigned G, unsigned B, unsigned A>
    struct SelectColorIndexRHS4<1, R, G, B, A> {
      enum { res = G };
    };

    template <unsigned R, unsigned G, unsigned B, unsigned A>
    struct SelectColorIndexRHS4<2, R, G, B, A> {
      enum { res = B };
    };

    template <unsigned R, unsigned G, unsigned B, unsigned A>
    struct SelectColorIndexRHS4<3, R, G, B, A> {
      enum { res = A };
    };

    template<unsigned R, unsigned G, unsigned B, unsigned A>
    struct RGBAValue_bad_color_indices: staticAssert::AssertBool<(
      R < 4 && G < 4 && B < 4 && A < 4 &&
      ((1 << R) + (1 << G) + (1 << B) + (1 << A) == 15)
    )> {};

  } // namespace detail

  // -------------------------------------------------------------------------- //
  // RGBAValue
  // -------------------------------------------------------------------------- //
  template<class ChannelType, unsigned redIndex = 0, unsigned greenIndex = 1, unsigned blueIndex = 2, unsigned alphaIndex = 3>
  class RGBAValue: public TinyVector<ChannelType, 4> {
    typedef TinyVector<ChannelType, 4> Base;

    /** Inverse mapping from index to color. */
    enum {
      IDX0 = (redIndex == 0) ? 0 : (greenIndex == 0) ? 1 : (blueIndex == 0) ? 2 : 3,
      IDX1 = (redIndex == 1) ? 0 : (greenIndex == 1) ? 1 : (blueIndex == 1) ? 2 : 3,
      IDX2 = (redIndex == 2) ? 0 : (greenIndex == 2) ? 1 : (blueIndex == 2) ? 2 : 3,
      IDX3 = (redIndex == 3) ? 0 : (greenIndex == 3) ? 1 : (blueIndex == 3) ? 2 : 3
    };

  public:
    /** STL-compatible definition of valuetype. */
    typedef typename Base::value_type value_type;

    /** STL-compatible definition of iterator. */
    typedef typename Base::iterator iterator;

    /** STL-compatible definition of const iterator. */
    typedef typename Base::const_iterator const_iterator;

    /** Squared norm type (result of squaredManitude()). */
    typedef typename Base::SquaredNormType SquaredNormType;
        
    /** Norm type (result of magnitude()). */
    typedef typename Base::NormType NormType;

    /** Color index positions. */
    enum {
      RedIdx = redIndex,
      GreenIdx = greenIndex,
      BlueIdx = blueIndex,
      AlphaIdx = alphaIndex
    };

    /** Construct from explicit color values.
     *  \a first, \a second, \a third, \fourth are written in this order,
     *  irrespective of how the color indices are specified. */
    RGBAValue(value_type first, value_type second, value_type third, value_type fourth): Base(first, second, third, fourth) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    /** Construct gray value. */
    RGBAValue(value_type gray): Base(gray, gray, gray, gray) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    /** Construct from another sequence (must have length 4!). */
    template<class Iterator>
    RGBAValue(Iterator i, Iterator end): Base(i[0], i[1], i[2], i[3]) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    /** Default constructor (sets all components to 0). */
    RGBAValue(): Base(0, 0, 0, 0) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    RGBAValue(const RGBAValue& r): Base(r) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    RGBAValue& operator=(const RGBAValue& r) {
      Base::operator=(r);
      return *this;
    }

    /** Copy constructor. */
    template<class U, unsigned R, unsigned G, unsigned B, unsigned A>
    RGBAValue(const RGBAValue<U, R, G, B, A>& r): Base(
      detail::RequiresExplicitCast<value_type>::cast(r[detail::SelectColorIndexRHS4<IDX0, R, G, B, A>::res]),
      detail::RequiresExplicitCast<value_type>::cast(r[detail::SelectColorIndexRHS4<IDX1, R, G, B, A>::res]),
      detail::RequiresExplicitCast<value_type>::cast(r[detail::SelectColorIndexRHS4<IDX2, R, G, B, A>::res]),
      detail::RequiresExplicitCast<value_type>::cast(r[detail::SelectColorIndexRHS4<IDX3, R, G, B, A>::res])
    ) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    /** Copy assignment. */
    template<class U, unsigned R, unsigned G, unsigned B, unsigned A>
    RGBAValue& operator=(const RGBAValue<U, R, G, B>& r) {
      setRed(detail::RequiresExplicitCast<value_type>::cast(r.red()));
      setGreen(detail::RequiresExplicitCast<value_type>::cast(r.green()));
      setBlue(detail::RequiresExplicitCast<value_type>::cast(r.blue()));
      setAlpha(detail::RequiresExplicitCast<value_type>::cast(r.alpha()));
      return *this;
    }

    /** Construct from TinyVector. */
    RGBAValue(const TinyVector<value_type, 4>& r): Base(r) {
      VIGRA_STATIC_ASSERT((detail::RGBAValue_bad_color_indices<redIndex, greenIndex, blueIndex, alphaIndex>));
    }

    /** Assign TinyVector. */
    RGBAValue & operator=(const TinyVector<value_type, 4>& r) {
      Base::operator=(r);
      return *this;
    }

    /** Unary negation (construct RGBValue with negative values). */
    RGBAValue operator-() const {
      RGBAValue result;
      result.setRed(-red());
      result.setGreen(-green());
      result.setBlue(-blue());
      result.setAlpha(-alpha());
      return result;
    }

    /** Access red component. */
    value_type & red() { return (*this)[redIndex]; }

    /** Access green component. */
    value_type & green() { return (*this)[greenIndex]; }

    /** Access blue component. */
    value_type & blue() { return (*this)[blueIndex]; }

    /** Access alpha component. */
    value_type & alpha() { return (*this)[alphaIndex]; }

    /** Get red component. */
    const value_type& red() const { return (*this)[redIndex]; }

    /** Get green component. */
    const value_type& green() const { return (*this)[greenIndex]; }

    /** Get blue component. */
    const value_type& blue() const { return (*this)[blueIndex]; }

    /** Get alpha component. */
    const value_type& alpha() const { return (*this)[alphaIndex]; }

    /** Calculate luminance. */
    value_type luminance() const {
    return detail::RequiresExplicitCast<value_type>::cast(0.3 * red() + 0.59 * green() + 0.11 * blue()); }

    /** Calculate magnitude (alpha is included into magnitude calculation). */
    NormType magnitude() const {
      return Base::magnitude();
    }

    /** Calculate squared magnitude (alpha is included into magnitude calculation). */
    SquaredNormType squaredMagnitude() const {
      return Base::squaredMagnitude();
    }

    /** Set red component. The type <TT>V</TT> of the passed in <TT>value</TT> is automatically converted to <TT>value_type</TT>. */
    template<class V>
    void setRed(V value) { (*this)[redIndex] = detail::RequiresExplicitCast<value_type>::cast(value); }

    /** Set green component.The type <TT>V</TT> of the passed in <TT>value</TT> is automatically converted to <TT>value_type</TT>. */
    template<class V>
    void setGreen(V value) { (*this)[greenIndex] = detail::RequiresExplicitCast<value_type>::cast(value); }

    /** Set blue component.The type <TT>V</TT> of the passed in <TT>value</TT> is automatically converted to <TT>value_type</TT>. */
    template<class V>
    void setBlue(V value) { (*this)[blueIndex] = detail::RequiresExplicitCast<value_type>::cast(value); }

    /** Set alpha component.The type <TT>V</TT> of the passed in <TT>value</TT> is automatically converted to <TT>value_type</TT>. */
    template<class V>
    void setAlpha(V value) { (*this)[alphaIndex] = detail::RequiresExplicitCast<value_type>::cast(value); }

    template<class V> 
    void setRGBA(V r, V g, V b, V a) {
        (*this)[redIndex] = detail::RequiresExplicitCast<value_type>::cast(r);
        (*this)[greenIndex] = detail::RequiresExplicitCast<value_type>::cast(g);
        (*this)[blueIndex] = detail::RequiresExplicitCast<value_type>::cast(b);
        (*this)[alphaIndex] = detail::RequiresExplicitCast<value_type>::cast(a);
    }

  };

} // namespace vigra

#endif // __ARX_EXT_VIGRA_RGBA_VALUE_H__
