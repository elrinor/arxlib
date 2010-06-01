#ifndef ARX_EXT_MAGICK_H
#define ARX_EXT_MAGICK_H

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

#endif // ARX_EXT_MAGICK_H
