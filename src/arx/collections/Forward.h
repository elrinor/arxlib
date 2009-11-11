#ifndef __ARX_FORWARD_H__
#define __ARX_FORWARD_H__

#include <boost/preprocessor.hpp>
#include "../Preprocessor.h"

#define ARX_FORWARD_CONSTRUCTOR_III(R, _, ELEM) , ELEM

#define ARX_FORWARD_CONSTRUCTOR_II(I, TO, FROM, INIT_SEQ, PAREN_BODY)           \
  BOOST_PP_IF(BOOST_PP_EQUAL(I, 0), BOOST_PP_EMPTY, BOOST_PP_IDENTITY(template<))() \
  BOOST_PP_ENUM_PARAMS(I, class T)                                              \
  BOOST_PP_IF(BOOST_PP_EQUAL(I, 0), BOOST_PP_EMPTY, BOOST_PP_IDENTITY(>))()     \
  TO(BOOST_PP_ENUM_BINARY_PARAMS(I, const T, & arg)):                           \
  FROM(BOOST_PP_ENUM_PARAMS(I, arg))                                            \
  BOOST_PP_SEQ_FOR_EACH(ARX_FORWARD_CONSTRUCTOR_III, ~, INIT_SEQ)               \
  ARX_STRIP PAREN_BODY

#define ARX_FORWARD_CONSTRUCTOR_I(Z, I, D)                                      \
  ARX_FORWARD_CONSTRUCTOR_II(I, BOOST_PP_TUPLE_ELEM(4, 0, D), BOOST_PP_TUPLE_ELEM(4, 1, D), BOOST_PP_TUPLE_ELEM(4, 2, D), BOOST_PP_TUPLE_ELEM(4, 3, D))

#define ARX_FORWARD_CONSTRUCTOR(TO, FROM, INIT_SEQ, PAREN_BODY)                 \
  BOOST_PP_REPEAT(10, ARX_FORWARD_CONSTRUCTOR_I, (TO, FROM, INIT_SEQ, PAREN_BODY))


#endif // __ARX_FORWARD_H__
