#include <iostream>

#include <boost/array.hpp>
#include <array>
#include <map>

#include <arx/collections/ExtenderFacade.h>

#include <arx/Utility.h>

int main() {
  try {
  Unreachable();

  typedef std::map<int, int> T;

  T v;

  arx::ExtenderFacade<T> f(v);

  f[0] = 1;


  std::cout << f.size() << f[0] << std::endl;

  } catch (std::runtime_error& e) {
    int a = 1;
  }

  return 0;
}