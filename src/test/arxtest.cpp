#pragma warning(disable:4996)  /* C4996: The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name. */

#define BOOST_TEST_MODULE arxtest
#include <boost/test/included/unit_test.hpp>

#include <vector>

#include <boost/range.hpp>

#include <arx/HasTypedXxx.h>

using namespace std;

typedef vector<int>::const_iterator it;


#define ARX_TRY_FORWARD((acc1, sig1)(acc2, sig2), ret, (ptype1, pname1)(ptype2, pname2))


ARX_HAS_TYPED_XXX_TRAIT_NAMED_DEF(has_insert, &T::insert, void (T::*)(it, int, int))


BOOST_AUTO_TEST_CASE(test_interface_category) {
  /*BOOST_CHECK_EQUAL(arx::detail::interface_category_impl::is_collection<std::vector<int> >::value, true);
  BOOST_CHECK_EQUAL(arx::detail::interface_category_impl::is_collection<int>::value, false);*/


  bool a = has_insert<vector<int> >::value;
  /*bool b = tmp::has_gbegin<std::vector<int> >::value;
  bool c = tmp::has_gbegin<std::pair<int*, int*> >::value;
  bool d = tmp::has_gbegin<std::pair<int, int> >::value;*/
}
