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
#define BOOST_TEST_MODULE arx_scope_exit
#include <boost/test/included/unit_test.hpp>

#include <arx/ScopeExit.h>

BOOST_AUTO_TEST_CASE(arx_scope_exit) {

  int i = 0;
  {
    ARX_SCOPE_EXIT(&) {
      i++;
    };

    BOOST_CHECK_EQUAL(i, 0);
  }
  BOOST_CHECK_EQUAL(i, 1);

}
