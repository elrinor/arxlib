#ifndef __ARX_EXT_MAGICK_H__
#define __ARX_EXT_MAGICK_H__

#include "config.h"
#include <Magick++.h>

// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_MAGICK
#  define ARX_USE_MAGICK
#endif

#ifdef ARX_USE_VIGRA
#  include "VigraMagick.h"
#endif

#endif // __ARX_EXT_MAGICK_H__
