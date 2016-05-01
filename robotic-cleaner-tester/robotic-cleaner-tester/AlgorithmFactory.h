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

#include <cstring>
#include <sstream> 
#include <fstream>

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "FactoryDefinition.h"  //header file where factory is defined


using namespace std;

class AlgorithmFactory{

	typedef list<void *>::iterator dl_itr;
	typedef map<string, maker_t *, less<string> >::iterator f_itr;
	typedef vector<AbstractAlgorithm *>::iterator algo_itr;
private:
	list<void *> dl_list; // list to hold handles for dynamic libs
	vector<string> algorithmNames; 
	map<string, string> _badAlgoMap;
public:
	AlgorithmFactory(){}
	~AlgorithmFactory();

	void tryAlgorithms();
	vector<AbstractAlgorithm *> CreateSetOfAllAlgorithms(); //: Create a set of all the algorithms we have constructors for.
	int ReadAlgorithms(vector<string> algorithmFiles); 
	
	vector<string> GetAlgorithmNames();
	map<string,string> GetBadAlgoMap();
	//void SetBadAlgoMap(map<string,string> errorMap);
	string ExtractFileName (string toSplit);
	string GetRidOfSuffix (string fileName);
	
}; //end of class

inline AlgorithmFactory::~AlgorithmFactory()
{
	for(dl_itr itr=dl_list.begin(); itr!=dl_list.end(); itr++){
		dlclose(*itr);
	}
	//cout << "all deleted" << endl;
}

inline vector<string> AlgorithmFactory::GetAlgorithmNames()
{
  return algorithmNames;
}

inline map<string,string> AlgorithmFactory::GetBadAlgoMap()
{
  return _badAlgoMap;
}

// inline void AlgorithmFactory::SetBadAlgoMap(map<string,string> errorMap)
// {
//   _badAlgoMap = errorMap;
// }
#endif /* ALGORITHMFACTORY_H_ */
