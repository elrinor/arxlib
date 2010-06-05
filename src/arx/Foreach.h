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
  for(bool ARX_FOREACH_VAR(_continue) = true; false;)                           \
  foreach(auto pair, MAP)                                                       \
  if(!ARX_FOREACH_VAR(_continue)) { break; } else                               \
  for(ARX_FOREACH_VAR(_continue) = false; false;)                               \
  for(KEY = pair.first; false;)                                                 \
  for(VAL = pair.second; false; ARX_FOREACH_VAR(_continue) = true)

#endif // ARX_FOREACH_H
