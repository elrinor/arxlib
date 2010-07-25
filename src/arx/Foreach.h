#ifndef ARX_FOREACH_H
#define ARX_FOREACH_H

#include <boost/foreach.hpp>
#include <boost/preprocessor/cat.hpp>

/* Undefine qt foreach. */
#ifdef foreach
#  undef foreach
#endif

#define foreach BOOST_FOREACH

#define reverse_foreach BOOST_REVERSE_FOREACH

#define ARX_FOREACH_VAR(NAME) BOOST_PP_CAT(arx_foreach_, BOOST_PP_CAT(NAME, __LINE__))

#define map_foreach(KEY, VAL, MAP)                                              \
  if(bool ARX_FOREACH_VAR(_continue) = true)                                    \
  foreach(auto pair, MAP)                                                       \
  if(!ARX_FOREACH_VAR(_continue)) break; else                                   \
  if(bool ARX_FOREACH_VAR(_iteration) = true)                                   \
  if((ARX_FOREACH_VAR(_continue) = false), true)                                \
  for(KEY = pair.first; ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_iteration) = false) \
  for(VAL = pair.second; ARX_FOREACH_VAR(_iteration); ARX_FOREACH_VAR(_continue) = true, ARX_FOREACH_VAR(_iteration) = false)

#endif // ARX_FOREACH_H
