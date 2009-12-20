#include <iostream>

#include <boost/array.hpp>
#include <array>
#include <map>

#include <arx/collections/ExtenderFacade.h>

int main() {
  typedef std::map<int, int> T;

  T v;

  arx::ExtenderFacade<T> f(v);

  f[0] = 1;


  std::cout << f.size() << f[0] << std::endl;

  return 0;
}