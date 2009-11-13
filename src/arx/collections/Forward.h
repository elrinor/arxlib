#ifndef __ARX_FORWARD_H__
#define __ARX_FORWARD_H__

#include <boost/preprocessor.hpp>
#include "../Preprocessor.h"

#define ARX_FWD_NOTHING_I(I)
#define ARX_FWD_LPAREN_I(I) (
#define ARX_FWD_RPAREN_I(I) )
#define ARX_FWD_COMMA_I(I) ,
#define ARX_FWD_ARGS_I(I) (BOOST_PP_ENUM_PARAMS(I, arg))

#define ARX_FWD_TEXT(TEXT) _FWD_NOTHING_I(~) TEXT ARX_FWD_NOTHING_
#define ARX_FWD_NOTHING() _FWD_NOTHING_
#define ARX_FWD_LPAREN() _FWD_LPAREN_
#define ARX_FWD_RPAREN() _FWD_RPAREN_
#define ARX_FWD_COMMA() _FWD_COMMA_
#define ARX_FWD_ARGS() _FWD_ARGS_

#define ARX_FORWARD_CONSTRUCTOR_III(r, index, elem)                             \
  ARX_CAT_3(ARX, elem, I)(index)

#define ARX_FORWARD_CONSTRUCTOR_II(index, classname, initSeq, parenBody)        \
  BOOST_PP_IF(BOOST_PP_EQUAL(index, 0), BOOST_PP_EMPTY, BOOST_PP_IDENTITY(template<))() \
  BOOST_PP_ENUM_PARAMS(index, class T)                                          \
  BOOST_PP_IF(BOOST_PP_EQUAL(index, 0), BOOST_PP_EMPTY, BOOST_PP_IDENTITY(>))() \
  classname(BOOST_PP_ENUM_BINARY_PARAMS(index, const T, & arg)):                \
  BOOST_PP_SEQ_FOR_EACH(ARX_FORWARD_CONSTRUCTOR_III, index, initSeq)            \
  ARX_STRIP(parenBody)

#define ARX_FORWARD_CONSTRUCTOR_I(z, index, data)                               \
  ARX_FORWARD_CONSTRUCTOR_II(index, BOOST_PP_TUPLE_ELEM(3, 0, data), BOOST_PP_TUPLE_ELEM(3, 1, data), BOOST_PP_TUPLE_ELEM(3, 2, data))

#define ARX_FORWARD_CONSTRUCTOR(classname, initSeq, parenBody)                \
  BOOST_PP_REPEAT(10, ARX_FORWARD_CONSTRUCTOR_I, (classname, initSeq, parenBody))

/**
 * Example usage:
 * ARX_FORWARD_CONSTRUCTOR(OMG, (ARX_FWD_TEXT(map))(ARX_FWD_ARGS())(ARX_FWD_COMMA())(ARX_FWD_TEXT(mInt(0))), ({}));
 */

#endif // __ARX_FORWARD_H__
