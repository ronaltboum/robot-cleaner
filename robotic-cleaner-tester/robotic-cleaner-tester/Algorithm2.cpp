#include <iostream> 
#include "Algorithm2.h"
using namespace std;

extern "C" {
AbstractAlgorithm *maker(){
   return new Algorithm2;
}
class proxy { public:
   proxy(){
      // register the maker with the factory 
      factory["Algorithm2"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}
