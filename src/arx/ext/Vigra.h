#ifndef __ARX_EXT_VIGRA_H__
#define __ARX_EXT_VIGRA_H__

#include "config.h"
#include "vigra/Accessors.h"
#include "vigra/Binarization.h"
#include "vigra/Colors.h"
#include "vigra/Convert.h"
#include "vigra/Converter.h"
#include "vigra/Drawing.h"
#include "vigra/MetaFunctions.h"
#include "vigra/RGBAValue.h"
#include "vigra/StdImage.h"
#include "vigra/Validation.h"

// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_VIGRA
#  define ARX_USE_VIGRA
#endif

#ifdef ARX_USE_MAGICK
#  include "VigraMagick.h"
#endif

#ifdef ARX_USE_OPENCV
#  include "VigraOpenCV.h"
#endif

#ifdef ARX_USE_QT
#  include "VigraQt.h"
#endif

#endif // __ARX_EXT_VIGRA_H__
