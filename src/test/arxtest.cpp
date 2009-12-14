#pragma warning(disable:4996)  /* C4996: The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name. */
#define BOOST_TEST_MODULE arxtest
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <arx/Collections.h>

ARX_HAS_FUNC_XXX_TRAIT_DEF(find)

BOOST_AUTO_TEST_CASE(arxtest) {
  /* ARX_HAS_FUNC_XXX_TRAIT_DEF tests. */
  BOOST_CHECK(has_find<std::set<float> >::value);
  BOOST_CHECK(!has_find<std::deque<float> >::value);


}
