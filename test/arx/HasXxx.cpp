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
#define BOOST_TEST_MODULE arx_has_xxx
#include <arx/ext/boost/Test.h>

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
  void f(C6);
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
  void g();
  void g(char *, const int &);

  friend struct D2;
};

struct D2 {
  ARX_DEFINE_HAS_FUNC_TRAIT(g);
};

struct E1 {
  void f(char);
  void f(int);
};

struct E2: E1 {
  using E1::f;

  void f(float);
};

struct F1 {
  void f();
};

struct F2: F1 {
  using F1::f;

  int f();
};

struct G2;

struct G1 {
  operator int () const;
};

struct G2 {
  G2(G1 *);

  void f(const G2 &);
  void f(int);
};



BOOST_AUTO_TEST_CASE(arx_has_xxx) {
  /* Has-type tests. */
  BOOST_CHECK(has_type<C1>::value);
  BOOST_CHECK(!has_type<C2>::value);
  BOOST_CHECK(!has_type<int>::value);

  /* Any-function tests. */
  BOOST_CHECK(has_f<C3>::value);
  BOOST_CHECK(!has_f<C4>::value);
  BOOST_CHECK(!has_f<char *>::value);
  
  /* Should detect protected and private functions. */
  BOOST_CHECK(has_f<C5>::value); 

  /* Const & non-const conversions. */
  BOOST_CHECK((!has_f<C3, void (C3::*)()>::value));
  BOOST_CHECK((has_f<C6, void (C6::*)() const>::value));
  BOOST_CHECK((!has_f<C6, void (C6::*)(C6) const>::value));
  
  /* Multiple arguments. */
  BOOST_CHECK((has_f<C6, void (C6::*)(int, float, char)>::value));
  BOOST_CHECK((has_f<C6, void (C6::*)(int, float, char) const>::value));
  BOOST_CHECK((!has_f<C3, void (C3::*)(int, float, char)>::value));

  /* Private member introspection, should compile. */
  BOOST_CHECK((D2::has_g<D1, void (D1::*)()>::value)); 
  BOOST_CHECK((D2::has_g<D1, void (D1::*)(char *, int)>::value));

  /* Mixing functions from different classes with using directive, should
   * compile. */
  BOOST_CHECK((has_f<E2, void (E2::*)(int)>::value)); 

  /* Ambiguous calls, should compile. */
  BOOST_CHECK((has_f<F2, void (F2::*)()>::value)); 

  /* Ambiguous base members should cause compilation error. 
   * BOOST_CHECK((!has_f<C9, void (C9::*)()>::value)); 
   */

  /* Should work with user-defined conversions. */
  BOOST_CHECK((has_f<G2, void (G2::*)(G1 *)>::value));
  BOOST_CHECK((has_f<G2, void (G2::*)(G1)>::value));
}
