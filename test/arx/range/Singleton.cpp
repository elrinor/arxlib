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
 * $Id: Foreach.cpp 156 2010-09-23 14:59:40Z ru.elric $ */
#define NOMINMAX
#define BOOST_TEST_MODULE arx_foreach
#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <arx/Foreach.h>
#include <arx/range/Singleton.h>

BOOST_AUTO_TEST_CASE(arx_range_singleton) {
  {
    int i = 0;
    foreach(int j, arx::singleton_range<const int>(10))
      i += j;
    BOOST_CHECK_EQUAL(i, 10);
  }

  {
    std::vector<int> vv;
    auto range = arx::singleton_range<std::vector<int> &>(vv);
    foreach(std::vector<int> &v, range)
      v.push_back(10);
    BOOST_CHECK_EQUAL(vv.size(), 1);
  }
}
