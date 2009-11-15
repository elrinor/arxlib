#ifndef __ARX_STATICCOUNTER_H__
#define __ARX_STATICCOUNTER_H__

#include "config.h"
#include <boost/preprocessor/cat.hpp>

/**
 * Defines a new static counter with initial value of 0.
 *
 * @param id identifier for a new counter. Must be either a number or a well-formed c++ identifier.
 */
#define ARX_NEW_COUNTER(id)                                                     \
  template<int N> class BOOST_PP_CAT(counter, id):                              \
    public BOOST_PP_CAT(counter, id)<N - 1> {};                                 \
  template<> class BOOST_PP_CAT(counter, id)<0> {};                             \
  char (*arx_test_counter(const BOOST_PP_CAT(counter, id)<0> *))[1];

/**
 * Returns the value of a static counter identified by id.
 */
#define ARX_GET_COUNTER(id) (sizeof(*arx_test_counter(static_cast<BOOST_PP_CAT(counter, id)<200>*>(NULL))) - 1)

/**
 * Increments the value of a static counter identified by id.
 * Must be used in the same namespace where ARX_NEW_COUNTER was used.
 */
#define ARX_INC_COUNTER(id) char (*arx_test_counter(const BOOST_PP_CAT(counter, id)<ARX_GET_COUNTER(id) + 1>*))[ARX_GET_COUNTER(id) + 2];

#endif // __ARX_STATICCOUNTER_H__
