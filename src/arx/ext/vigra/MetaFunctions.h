#ifndef __ARX_EXT_VIGRA_META_FUNCTIONS_H__
#define __ARX_EXT_VIGRA_META_FUNCTIONS_H__

#include "config.h"
#include <boost/mpl/int.hpp>
#include <boost/mpl/identity.hpp>
#include <vigra/rgbvalue.hxx>

namespace vigra {
// -------------------------------------------------------------------------- //
// channels
// -------------------------------------------------------------------------- //
  template<class PixelType> struct channels: boost::mpl::int_<1> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
  struct channels<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: boost::mpl::int_<3> {};

  template<class ChannelType> 
  struct channels<TinyVector<ChannelType, 4> >: boost::mpl::int_<4> {};


// -------------------------------------------------------------------------- //
// channel_type
// -------------------------------------------------------------------------- //
  template<class PixelType> struct channel_type: boost::mpl::identity<PixelType> {};

  template<class ChannelType, unsigned redIndex, unsigned greenIndex, unsigned blueIndex> 
  struct channel_type<RGBValue<ChannelType, redIndex, greenIndex, blueIndex> >: boost::mpl::identity<ChannelType> {};

  template<class ChannelType> 
  struct channel_type<TinyVector<ChannelType, 4> >: boost::mpl::identity<ChannelType> {};

} // namespace vigra

#endif // __ARX_EXT_VIGRA_META_FUNCTIONS_H__
