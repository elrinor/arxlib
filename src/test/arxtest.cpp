#pragma warning(disable:4996)  /* C4996: The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name. */
/*#define BOOST_TEST_MODULE arxtest
#include <boost/test/included/unit_test.hpp>
*/

#include <iostream>

#include <deque>

#include <arx/Collections.h>

ARX_HAS_FUNC_XXX_TRAIT_DEF(find)


int main() {


  arx::set<int> s;
  arx::map<int, int> m;
  arx::vector<int> v;

  std::cout << has_find<std::set<float> >::value;
  std::cout << has_find<std::deque<float> >::value;


  return 0;
}
