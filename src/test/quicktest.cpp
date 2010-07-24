#include <iostream>

#include <boost/array.hpp>
#include <array>
#include <map>

#include <arx/collections/ExtenderFacade.h>

#include <arx/Utility.h>

#include <arx/ext/vigra/RGBAValue.h>

#include <arx/ext/Vigra.h>

int main() {

  vigra::RGBValue<char> v;
  -v;

  return 0;
}