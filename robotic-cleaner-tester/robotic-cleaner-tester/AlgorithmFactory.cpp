#include "stdafx.h"
#include "AlgorithmFactory.h"
#include "AlgorithmFactory.h"
#include <list>
#include <vector>

namespace ns_robotic_cleaner_simulator{

map<string, maker_t *, less<string> > factory;
	
AlgorithmFactory::AlgorithmFactory(void)
{
	factory = map<string, maker_t *, less<string> >();
	 _dynamicLibs = vector<void *>();
	 _alogrithmNames = vector<string>(); 
}


AlgorithmFactory::~AlgorithmFactory(void)
{
	vector<void *>::iterator _dlIterator; 
	vector<AbstractAlgorithm *>::iterator _algoIterator; 
	// close all the dynamic libs we opened
	for(_dlIterator=_dynamicLibs.begin(); _dlIterator!=_dynamicLibs.end(); _dlIterator++){
		dlclose(*_dlIterator);
	}
}
//************************************
// Brief:		Gets _ and returns
// Gets:	 	string path
// Returns:   	void
// Pre:			-
// Post:		-
//************************************
void AlgorithmFactory::ReadAlgorithms(string algorithmsPath /* = "./" */)
{
	vector<void *>::iterator dlIterator; 
	vector<AbstractAlgorithm *>::iterator algoIterator; 
	map<string, maker_t *, less<string> >::iterator factoryIterator;
	FILE *dl;   // handle to read directory 
	const char *command_str = "ls *.so";  // command string to get dynamic lib names
	char in_buf[BUF_SIZE]; // input buffer for lib names 
	// get the names of all the dynamic libs (.so  files) in the current dir 
	dl = popen(command_str, "r"); 
	if(!dl){
		perror("popen");
		exit(-1);
	}
	void *dlib; 
	char name[1024]; 
	while(fgets(in_buf, BUF_SIZE, dl)){
		// trim off the whitespace 
		char *ws = strpbrk(in_buf, " \t\n"); 
		if(ws) *ws = '\0';
		// append ./ to the front of the lib name
		sprintf(name, "./%s", in_buf); 
		dlib = dlopen(name, RTLD_NOW);
		if(dlib == NULL){
			cerr << dlerror() << endl; 
			exit(-1);
		}
		// add the handle to our list
		_dynamicLibs.insert(_dynamicLibs.end(), dlib);
	}
	// create an array of the AbstractAlgorithm names
	for(factoryIterator=factory.begin() ;  factoryIterator!=factory.end() ; factoryIterator++){
		_alogrithmNames.push_back(factoryIterator->first);
	}
}

//************************************
// Brief:		Create a set of all the algorithms we have constructors for.
// Returns:   	vector<AbstractAlgorithm *>
// Pre:			-
// Post:		-
//************************************
vector<AbstractAlgorithm *> AlgorithmFactory::CreateSetOfAllAlgorithms()
{
	vector<AbstractAlgorithm *> algorithms = vector<AbstractAlgorithm *>();
	map<string, maker_t *, less<string> >::iterator factoryIterator;
	for(factoryIterator=factory.begin(); factoryIterator!=factory.end(); factoryIterator++){
		algorithms.push_back(factoryIterator->second());
	}
	return algorithms; 
}



}