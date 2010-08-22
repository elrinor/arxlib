#ifndef ARX_PREPROCESSOR_H
#define ARX_PREPROCESSOR_H

#include "config.h"
#include <boost/preprocessor/cat.hpp>

#include <boost/preprocessor/for.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/comparison/not_equal.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>

#define ARX_STRIP_I(...) __VA_ARGS__
#define ARX_STRIP(ARG) ARX_STRIP_I ARG

#define ARX_CAT_3(a, b, c) BOOST_PP_CAT(BOOST_PP_CAT(a, b), c)
#define ARX_CAT_4(a, b, c, d) ARX_CAT_3(BOOST_PP_CAT(a, b), c, d)


#define ARX_NUMERIC_FOR_OP(R, STATE)                                            \
  (                                                                             \
    BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(4, 0, STATE)),                             \
    BOOST_PP_TUPLE_ELEM(4, 1, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 2, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 3, STATE)                                            \
  )

#define ARX_NUMERIC_FOR_PRED(R, STATE)                                          \
  BOOST_PP_NOT_EQUAL(                                                           \
    BOOST_PP_TUPLE_ELEM(4, 0, STATE),                                           \
    BOOST_PP_TUPLE_ELEM(4, 1, STATE)                                            \
  )

#define ARX_NUMERIC_FOR_MACRO(R, STATE)                                         \
  BOOST_PP_TUPLE_ELEM(4, 3, STATE)(R, BOOST_PP_TUPLE_ELEM(4, 2, STATE), BOOST_PP_TUPLE_ELEM(4, 0, STATE))

/**
 * Performs a horizontal repetition which can best be described with the
 * following pseudocode:
 *
 * for(int i = MIN; i != MAX; i++)
 *   MACRO(R, DATA, i);
 */
#define ARX_NUMERIC_FOR(MIN, MAX, DATA, MACRO)                                  \
  BOOST_PP_FOR((MIN, MAX, DATA, MACRO), ARX_NUMERIC_FOR_PRED, ARX_NUMERIC_FOR_OP, ARX_NUMERIC_FOR_MACRO)
  

#endif // ARX_PREPROCESSOR_H
