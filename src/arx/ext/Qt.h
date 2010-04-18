#ifndef __ARX_EXT_QT_H__
#define __ARX_EXT_QT_H__

#include "config.h"

// -------------------------------------------------------------------------- //
// Inter-lib defines
// -------------------------------------------------------------------------- //
#ifndef ARX_USE_QT
#  define ARX_USE_QT
#endif

#ifdef ARX_USE_VIGRA
#  include "VigraQt.h"
#endif

#endif // __ARX_EXT_QT_H__
