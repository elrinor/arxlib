#ifndef __ARX_EXT_VIGRA_STD_IMAGE_H__
#define __ARX_EXT_VIGRA_STD_IMAGE_H__

#include "config.h"
#include <vigra/stdimage.hxx>
#include <vigra/rgbvalue.hxx>
#include "RGBAValue.h"

namespace vigra {
  typedef BasicImage<RGBValue<UInt8, 2, 1, 0> > BBGRImage;
  typedef BasicImage<RGBAValue<UInt8> > BRGBAImage;
  typedef BasicImage<RGBAValue<UInt8, 2, 1, 0, 3> > BBGRAImage;

} // namespace vigra

#endif // __ARX_EXT_VIGRA_STD_IMAGE_H__
