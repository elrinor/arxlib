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

ARX_DEFINE_NAMED_HAS_TYPE_TRAIT(has_type, type);


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

ARX_DEFINE_HAS_FUNC_TRAIT(f);


struct C7 {
  void f();
};

struct C8 {
  void f(int);
};

struct C9: C7, C8 {};

struct D1 {
private:
  void f();

  friend struct D2;
};

struct D2 {
  ARX_DEFINE_HAS_FUNC_TRAIT(f);
};

BOOST_AUTO_TEST_CASE(arx_has_xxx) {

  BOOST_CHECK(has_type<C1>::value);
  BOOST_CHECK(!has_type<C2>::value);
  BOOST_CHECK(!has_type<int>::value);


  BOOST_CHECK(has_f<C3>::value);
  BOOST_CHECK(!has_f<C4>::value);
  BOOST_CHECK(!has_f<char *>::value);

  BOOST_CHECK(has_f<C5>::value); /* Private members must also be detected. */

  BOOST_CHECK((!has_f<C3, void (C3::*)()>::value));
  BOOST_CHECK((has_f<C6, void (C6::*)() const>::value));
  BOOST_CHECK((!has_f<C9, void (C9::*)()>::value)); /* Ambiguous members don't cause a compilation error. */

  BOOST_CHECK((!has_f<C6, void (C6::*)(int, float, char)>::value));
  BOOST_CHECK((has_f<C6, void (C6::*)(int, float, char) const>::value));
  BOOST_CHECK((!has_f<C3, void (C3::*)(int, float, char)>::value));

  BOOST_CHECK((D2::has_f<D1, void (D1::*)()>::value)); /* Private member introspection. */
}
