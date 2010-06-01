#ifndef ARX_EXT_QT_H
#define ARX_EXT_QT_H

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

#endif // ARX_EXT_QT_H
