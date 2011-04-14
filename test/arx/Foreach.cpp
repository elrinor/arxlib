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
#define BOOST_TEST_MODULE arx_foreach
#include <arx/ext/boost/Test.h>

#include <vector>
#include <map>
#include <algorithm>
#include <arx/Foreach.h>

BOOST_AUTO_TEST_CASE(arx_foreach) {

  /* Tests for map_foreach. Checks break & continue. */
  {
    std::map<int, int> m;
    m[0] = 1;
    m[1] = 10;
    m[2] = 100;
    m[3] = 1000;

    int sum = 0, keysum = 0;
    map_foreach(int key, int value, m) {
      sum += value;
      keysum += key;
    }
    BOOST_CHECK_EQUAL(sum, 1111);
    BOOST_CHECK_EQUAL(keysum, 6);

    sum = 0;
    map_foreach(int key, int value, m) {
      if(key < 2)
        continue;
      sum += value;
    }
    BOOST_CHECK_EQUAL(sum, 1100);

    sum = 0;
    int max = 0;
    map_foreach(int key, int value, m) {
      if(value > 100)
        break;
      max = std::max(max, value);
    }
    BOOST_CHECK(max <= 100);

  }

}
