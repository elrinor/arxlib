#ifndef __ARX_FOREACH_H__
#define __ARX_FOREACH_H__

#include <boost/foreach.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/preprocessor/cat.hpp>

/* Undefine qt foreach. */
#ifdef foreach
#  undef foreach
#endif

#define foreach BOOST_FOREACH

#define reverse_foreach BOOST_REVERSE_FOREACH

#define ARX_FOREACH_VAR(NAME) BOOST_PP_CAT(NAME, __LINE__)

#define map_foreach(KEY, VAL, MAP)                                              \
  BOOST_FOREACH(BOOST_TYPEOF(*(MAP).begin())& pair, MAP)                        \
  for(bool ARX_FOREACH_VAR(stop) = false; !ARX_FOREACH_VAR(stop);)              \
  for(KEY = pair.first; !ARX_FOREACH_VAR(stop);)                                \
  for(VAL = pair.second; !ARX_FOREACH_VAR(stop); ARX_FOREACH_VAR(stop) = true)

#endif // __ARX_FOREACH_H__
