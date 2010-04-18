#ifndef __ARX_EXT_VIGRA_META_FUNCTIONS_H__
#define __ARX_EXT_VIGRA_META_FUNCTIONS_H__

#include "config.h"
#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <vigra/rgbvalue.hxx>
#include "RGBAValue.h"

namespace vigra {
// -------------------------------------------------------------------------- //
// channels
// -------------------------------------------------------------------------- //
  template<class PixelType> struct channels: boost::mpl::int_<1> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
  struct channels<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: boost::mpl::int_<3> {};

  template<class ChannelType, int size> 
  struct channels<TinyVector<ChannelType, size> >: boost::mpl::int_<size> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex, unsigned alphaIndex>
  struct channels<RGBAValue<ChannelType, redIndex, greenIndex, blueIndex, alphaIndex> >: boost::mpl::int_<4> {};


// -------------------------------------------------------------------------- //
// channel_type
// -------------------------------------------------------------------------- //
  template<class PixelType> struct channel_type: boost::mpl::identity<PixelType> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
  struct channel_type<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: boost::mpl::identity<ChannelType> {};

  template<class ChannelType, int size> 
  struct channel_type<TinyVector<ChannelType, size> >: boost::mpl::identity<ChannelType> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex, unsigned alphaIndex>
  struct channel_type<RGBAValue<ChannelType, redIndex, greenIndex, blueIndex, alphaIndex> >: boost::mpl::identity<ChannelType> {};

} // namespace vigra

#endif // __ARX_EXT_VIGRA_META_FUNCTIONS_H__
