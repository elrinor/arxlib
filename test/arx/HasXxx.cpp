/* This file is part of ArXLib, a C++ ArX Primitives Library.
 *
 * Copyright (C) 2008-2010 Alexander Fokin <apfokin@gmail.com>
 *
 * ArXLib is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * ArXLib is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with ArXLib. If not, see <http://www.gnu.org/licenses/>. 
 * 
 * $Id$ */
#define NOMINMAX
#define BOOST_TEST_MODULE arx_has_xxx
#include <boost/test/included/unit_test.hpp>

#include <arx/HasXxx.h>

struct C1 {
  typedef int type;
};

struct C2 {
  int type();
};

ARX_HAS_TYPE_XXX_TRAIT_NAMED_DEF(has_type, type);


struct C3 {
  void f(float);
  void f(int);
};

struct C4 {
  void g();
};

class C5 {
protected:
  void f();
};

struct C6 {
  void f(int, float, char) const;
  void f() const;
};

ARX_HAS_FUNC_XXX_TRAIT_DEF(f);


ARX_HAS_EXACT_FUNC_XXX_TRAIT_NAMED_DEF(has_f_void, f, void());
ARX_HAS_EXACT_FUNC_XXX_TRAIT_NAMED_DEF(has_f_int_float_char, f, void(int, float, char));


BOOST_AUTO_TEST_CASE(arx_has_xxx) {

  BOOST_CHECK(has_type<C1>::value);
  BOOST_CHECK(!has_type<C2>::value);
  BOOST_CHECK(!has_type<int>::value);


  BOOST_CHECK(has_f<C3>::value);
  BOOST_CHECK(!has_f<C4>::value);
  BOOST_CHECK(!has_f<char *>::value);

  BOOST_CHECK(has_f<C5>::value); /* Private members must also be detected. */

  BOOST_CHECK(!has_f_void<C3>::value);
  BOOST_CHECK(has_f_void<C6>::value);
  BOOST_CHECK(!has_f_void<int>::value);

  BOOST_CHECK(has_f_int_float_char<C6>::value);
  BOOST_CHECK(!has_f_int_float_char<C3>::value);
  BOOST_CHECK(!has_f_int_float_char<void *>::value);

}
