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
#ifndef ARX_EXT_VIGRA_META_FUNCTIONS_H
#define ARX_EXT_VIGRA_META_FUNCTIONS_H

#include <arx/config.h>
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

#endif // ARX_EXT_VIGRA_META_FUNCTIONS_H
