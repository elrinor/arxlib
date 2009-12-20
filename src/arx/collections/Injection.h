#ifndef __ARX_COLLECTIONS_INJECTION_H__
#define __ARX_COLLECTIONS_INJECTION_H__

#include "config.h"
#include <boost/preprocessor.hpp> /* for BOOST_PP_SEQ_FOR_EACH */

// -------------------------------------------------------------------------- //
// Type injection macros
// -------------------------------------------------------------------------- //
#define ARX_INJECT_TYPES_I(r, from, e)                                          \
  typedef from::e e;

#define ARX_INJECT_TYPES(from, type_list)                                       \
  BOOST_PP_SEQ_FOR_EACH(ARX_INJECT_TYPES_I, from, type_list)

#define ARX_INJECT_TYPES_TPL_I(r, from, e)                                      \
  typedef typename from::e e;

#define ARX_INJECT_TYPES_TPL(from, type_list)                                   \
  BOOST_PP_SEQ_FOR_EACH(ARX_INJECT_TYPES_TPL_I, from, type_list)

#endif // __ARX_COLLECTIONS_INJECTION_H__
