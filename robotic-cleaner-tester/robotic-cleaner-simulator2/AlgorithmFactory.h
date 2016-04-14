#ifndef ALGORITHMFACTORY_H_
#define ALGORITHMFACTORY_H_
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
#include "AbstractAlgorithm.h"
#include "Direction.h"

using namespace std;

class AlgorithmFactory{

	typedef list<void *>::iterator dl_itr;
	typedef map<string, maker_t *, less<string> >::iterator f_itr;
	typedef vector<AbstractAlgorithm *>::iterator algo_itr;
private:
	list<void *> dl_list; // list to hold handles for dynamic libs
	vector<string> algorithmNames;  // vector of shape types used to build menu
public:
	AlgorithmFactory(){}
	~AlgorithmFactory();

	void tryAlgorithms();
	vector<AbstractAlgorithm *> CreateSetOfAllAlgorithms(); //: Create a set of all the algorithms we have constructors for.
	int ReadAlgorithms(string algorithmsPath = "./"); //TODO: change so it will use path
}; //end of class

inline AlgorithmFactory::~AlgorithmFactory()
{
	for(dl_itr itr=dl_list.begin(); itr!=dl_list.end(); itr++){
		dlclose(*itr);
	}
	cout << "all deleted" << endl;
}

#endif /* ALGORITHMFACTORY_H_ */
