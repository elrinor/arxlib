#pragma warning(disable:4996)  /* C4996: The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name. */
#define BOOST_TEST_MODULE arxtest
#include <boost/test/included/unit_test.hpp>

#include <arx/Collections.h>

BOOST_AUTO_TEST_CASE(test_interface_category) {
  arx::vector<int> v;

  v.push_back(1);

  int sz = v.end() - v.begin();
  v.size();

  v.at(0);
  v[0];

  v.pop_back();
  v.push_back();
  v.insert(v.begin(), 100, 1);

  v << 2 << 3;

  arx::map<int, int> m;
  m[0] = 10;

  const arx::map<int, int>& g = m;

  m[0];

}
