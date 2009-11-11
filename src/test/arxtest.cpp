
#include <boost/typeof/typeof.hpp>
#include <boost/utility.hpp>

#include <arx/collections/MapFacade.h>
#include <unordered_map>
#include <map>

#include <arx/collections/Forward.h>

class OMG: public std::map<int, int> {
public:
  ARX_FORWARD_CONSTRUCTOR(OMG, map, (mInt(0)), ({}))

private:
  int mInt;

};

int main() {
  OMG m = OMG();

  return 0;
}