#include <iostream> 
#include "Algorithm1.h"
using namespace std;

extern "C" {
AbstractAlgorithm *maker(){
   return new Algorithm1;
}
class proxy { 
public:
   proxy(){
      // register the maker with the factory 
      factory["Algorithm1"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}
