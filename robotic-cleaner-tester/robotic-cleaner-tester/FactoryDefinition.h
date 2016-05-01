#ifndef __FACTORY_DEFINITION_H
#define __FACTORY_DEFINITION_H

#include <map>
#include <string>
#include "Direction.h"
#include "AbstractAlgorithm.h"
using namespace std;


// typedef to make it easier to set up our factory 
typedef AbstractAlgorithm *maker_t();
// our global factory 
extern std::map<std::string, maker_t *, std::less<std::string> > factory;

#endif //__FACTORY_DEFINITION_H