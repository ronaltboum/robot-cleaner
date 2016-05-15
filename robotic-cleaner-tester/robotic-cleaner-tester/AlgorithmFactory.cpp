#include "AlgorithmFactory.h"
#include "AbstractAlgorithm.h"
#define BUF_SIZE 1024

using namespace std;

// our global factory for making Algorithms
map<string, maker_t *, less<string> > factory;

int AlgorithmFactory::ReadAlgorithms(vector<string> algorithmFiles)
{

   dl_itr dlItr;
   f_itr fitr;
  
   vector<string>::iterator stringIt;
   vector<string> validSO;  //names of all so that are valid. later we check if each name is registered
   void *dlib;
   string errorMessage;
   for(stringIt=algorithmFiles.begin() ; stringIt!=algorithmFiles.end() ; stringIt++){
	string fullFileName = (*stringIt);
	string algoName = ExtractFileName (fullFileName);
	//const char* suffix = ".";
	string algoNoSuffix = GetRidOfSuffix (algoName);
	string appended;
	if(fullFileName.length() > 0){
	    if(fullFileName[0] != '/')
	       appended = "./" + (*stringIt);   // append ./ to the front of the lib name
	    else
		appended = (*stringIt);
	}
	else
	  appended = "./" + (*stringIt);   // append ./ to the front of the lib name

	dlib = dlopen( appended.c_str(), RTLD_NOW);

	//printf("%s\n", appended.c_str() );   //delete !!!!!!!!!!!!!!!!!

	if(dlib == NULL){
	    errorMessage = "file cannot be loaded or is not a valid .so";
	    _badAlgoMap.insert( pair<string, string>(algoNoSuffix, errorMessage) );
	 }
	 
	// add the handle to our list
	if(dlib != NULL)
	  dl_list.insert(dl_list.end(), dlib);
	
	validSO.push_back(algoNoSuffix);
   }
   
   int i = 0;
   // create an array of the AbstractAlgorithm names
   for(fitr=factory.begin(); fitr!=factory.end(); fitr++){
	  algorithmNames.insert(algorithmNames.end(), fitr->first);
	  i++;
   }
   
   //search in factory every algo name which is a valid .so.  if it's not in factory, then it isn't registered
   for(stringIt = validSO.begin() ; stringIt != validSO.end() ; stringIt++){
      fitr = factory.find(*stringIt);
      if ( fitr == factory.end() ) { // not found
	  errorMessage = "valid .so but no algorithm was registered after loading it";
	  //string withSuffix = (*stringIt) + ".so";
	  _badAlgoMap.insert( pair<string, string>( (*stringIt) , errorMessage) );
      }
   }
   
   return i;
}

void AlgorithmFactory::tryAlgorithms()
{
//	vector<AbstractAlgorithm *> algoSet = CreateSetOfAllAlgorithms();
//	vector<AbstractAlgorithm *>::iterator aItr;
//	for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
//		(*aItr)->setConfiguration(map<string, int>());
//		//Direction d = (*aItr)->step();
//		(*aItr)->step();
//		(*aItr)->aboutToFinish(0);
//	}
//	
//	for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
//		delete *aItr;
//	}
}


vector<AbstractAlgorithm *> AlgorithmFactory::CreateSetOfAllAlgorithms()
{
	vector<AbstractAlgorithm *> algorithmSet = vector<AbstractAlgorithm *>();
	vector<string>::iterator nItr;
	for(nItr=algorithmNames.begin(); nItr!=algorithmNames.end(); nItr++){
		algorithmSet.push_back(factory[*nItr]());
	}
	return algorithmSet;
}

string AlgorithmFactory::ExtractFileName (string toSplit)
{
  vector<string> words;
  //std::string str ("/ron/dir/simple one.house");

  char * cstr = new char [toSplit.length()+1];
  std::strcpy (cstr, toSplit.c_str());

  // cstr now contains a c-string copy of toSplit
  
  char * p = std::strtok (cstr,"/");
  stringstream myStreamString;
  myStreamString << p;
  string myString = myStreamString.str();
  //cout << myString << endl;
  words.push_back(myString);
  p = std::strtok(NULL, "/");
    
  while (p!=0)
  {
    //std::cout << p << '\n';
     stringstream myStreamString;
     myStreamString << p;
     string myString = myStreamString.str();
     //cout << myString << endl;
     words.push_back(myString);
     p = std::strtok(NULL,"/");
   
  }
  
  vector<string>::iterator strIt;
  delete[] cstr;
  //for (strIt = words.begin();  strIt != words.end(); ++strIt) {
    //cout << (*strIt) << endl;
  //}
  string last = words.back();  //simple1.house
  //cout<< last << endl;
  return last;
}

string AlgorithmFactory::GetRidOfSuffix (string toSplit)
{
  
  char * cstr = new char [toSplit.length()+1];
  std::strcpy (cstr, toSplit.c_str());
  // cstr now contains a c-string copy of toSplit
  char * p = std::strtok (cstr,".");
  stringstream myStreamString;
  myStreamString << p;
  string myString = myStreamString.str();
  
  delete[] cstr;
  return myString;
}
