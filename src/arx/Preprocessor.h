#ifndef __ARX_PREPROCESSOR_H__
#define __ARX_PREPROCESSOR_H__

#include "config.h"
#include <boost/preprocessor/cat.hpp>

#define ARX_STRIP_I(...) __VA_ARGS__
#define ARX_STRIP(ARG) ARX_STRIP_I ARG

#define ARX_CAT_3(a, b, c) BOOST_PP_CAT(BOOST_PP_CAT(a, b), c)
#define ARX_CAT_4(a, b, c, d) ARX_CAT_3(BOOST_PP_CAT(a, b), c, d)

#endif // __ARX_PREPROCESSOR_H__
