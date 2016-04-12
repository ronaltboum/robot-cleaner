#ifndef AlgorithmFactory_h__
#define AlgorithmFactory_h__

#include "AbstractAlgorithm.h"
#include <iostream> 
#include <map> 
#include <list> 
#include <vector>
#include <string> 
#include <dlfcn.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 1024

namespace ns_robotic_cleaner_simulator
{
	// typedef to make it easier to set up our factory 
	typedef AbstractAlgorithm *maker_t();
	// our global factory 
	extern std::map<std::string, maker_t *, std::less<std::string> > factory;

	class AlgorithmFactory
	{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		vector<void *> _dynamicLibs; // list to hold handles for dynamic libs 
		vector<string> _alogrithmNames;  // vector of AbstractAlgorithm types used to build menu

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		AlgorithmFactory(void);
		~AlgorithmFactory(void);

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		void ReadAlgorithms(string algorithmsPath = "./"); //TODO: change so it will use path
		vector<AbstractAlgorithm *> CreateSetOfAllAlgorithms(); //: Create a set of all the algorithms we have constructors for.
	};


	
} //end of namespace ns_robotic_cleaner_simulator

#endif // AlgorithmFactory_h__
