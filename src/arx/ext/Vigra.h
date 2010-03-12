#ifndef __ARX_EXT_VIGRA_H__
#define __ARX_EXT_VIGRA_H__

#include "config.h"
#include "vigra/Accessor.h"
#include "vigra/Binarization.h"
#include "vigra/Colors.h"
#include "vigra/Conversion.h"
#include "vigra/Drawing.h"
#include "vigra/MetaFunctions.h"
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

#endif // __ARX_EXT_VIGRA_H__
