#include "AlgorithmFactory.h"

//========================================================================
// AlgorithmFactory.cpp is part of the simulation project ONLY
// You have the freedom to implement it as you wish!
// You even don't have to have such a class!
//========================================================================
// create the static field
AlgorithmFactory AlgorithmFactory::instance;

//create a set of all the abstract algorithms
vector<AbstractAlgorithm *> AlgorithmFactory::CreateSetOfAllAlgorithms() const 
{
    vector<AbstractAlgorithm *> algorithms;
    for(auto algorithmFactoryFunc : algorithmFactories) {
        algorithms.push_back(algorithmFactoryFunc().release());
    }
    return algorithms;
}

//----------------------------------------------------------------------------
// AlgorithmFactory::
// Load the algoirthm so library
// return wheter the file was loaded successfully
//----------------------------------------------------------------------------
bool AlgorithmFactory::loadAlgorithm(const std::string& fullPath, const std::string& algoName) {
	// cout << "AlgorithmFactory::loadAlgorithm fullPath: " << fullPath << " algoName: " << algoName <<endl;
    string algoNameCopy = algoName;
	string printVersion = FixAlgoNameForPrint(algoNameCopy);  //orig name: _039563838_E.so      should print:   331332334_C_.so
	void *dlib;
    size_t size = instance.size();
    string errorMessage;
    // cout << "opening: " << fullPath << endl;
    dlib = dlopen( fullPath.c_str(), RTLD_NOW);
    if(dlib == NULL){
        errorMessage = "file cannot be loaded or is not a valid .so";
        _badAlgoMap.insert( pair<string, string>(printVersion, errorMessage) );
        return false;
    }
    if(instance.size() == size) {
        dlclose(dlib);
        errorMessage = "valid .so was loaded but no algorithm was registered";
        _badAlgoMap.insert( pair<string, string>(printVersion, errorMessage) );
        return false;
    }
    dl_list.insert(dl_list.end(), dlib);
    instance.setNameForLastAlgorithm(algoName);
    return true;
}


int AlgorithmFactory::ReadAlgorithms(vector<string> algorithmFiles)
{
  
    vector<string>::const_iterator stringIt;
    string errorMessage = "file cannot be loaded or is not a valid .so";;
    int loadedSuccessfully = 0;
    for(stringIt=algorithmFiles.begin() ; stringIt!=algorithmFiles.end() ; stringIt++){
        string fullFilePath = (*stringIt);
        string algoName = ExtractFileName (fullFilePath);
        //const char* suffix = ".";
        string algoNoSuffix = GetRidOfSuffix (algoName);
        if(fullFilePath.length() > 0 && fullFilePath[0] != '/'){
            fullFilePath = "./" + fullFilePath;   // append ./ to the front of the lib name
        }
        if(loadAlgorithm(fullFilePath, algoNoSuffix)){
            ++loadedSuccessfully;
        }
    }
    return loadedSuccessfully;
}

void AlgorithmFactory::tryAlgorithms()
{
//  vector<AbstractAlgorithm *> algoSet = CreateSetOfAllAlgorithms();
//  vector<AbstractAlgorithm *>::iterator aItr;
//  for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
//      (*aItr)->setConfiguration(map<string, int>());
//      //Direction d = (*aItr)->step();
//      (*aItr)->step();
//      (*aItr)->aboutToFinish(0);
//  }
//  
//  for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
//      delete *aItr;
//  }
}

string AlgorithmFactory::ExtractFileName (string filename)
{
    string cuttedString = filename;
    size_t idx = cuttedString.rfind('/');
    if(idx != std::string::npos)
    {
        cuttedString = cuttedString.substr(idx+1);
    }
    idx = cuttedString.rfind('\\');
    if(idx != std::string::npos)
    {
        cuttedString = cuttedString.substr(idx);
    }
    return cuttedString;
}

string AlgorithmFactory::GetRidOfSuffix (string filename)
{

    size_t idx = filename.rfind('.');
    if(idx != std::string::npos)
    {
        return filename.substr(0, idx);
    }
    return filename;
}

//orig name: _039563838_E.so      should print:   331332334_C_.so
//   /_039563838_E.so: valid .so was loaded but no algorithm was registered
//   /invalidAlgorithm.so
string AlgorithmFactory::FixAlgoNameForPrint(string algoName)
{
	int len = algoName.length();
	if(len <= 2)
		return algoName;  //shouldn't happen
	if(algoName.at(0) != '_')  {
		if(algoName.at(0) != '/') {
			return algoName;  //shouldn't happen
		}
		else {
			if(algoName.at(1) == '_') {  //case  /_039563838_E.so
				algoName = algoName.substr (2,len-2);
				vector<string> splitted = split(algoName, '.');
				algoName = splitted.at(0) + "_";
				return algoName;
			}
			else {  // case    /invalidAlgorithm.so
				algoName = algoName.substr (1,len-1);
				vector<string> splitted = split(algoName, '.');
				algoName = splitted.at(0);
				return algoName;
			}	
		}
	}
	
	algoName = algoName.substr (1,len-1);   //algoName is now 039563838_E.so
	vector<string> splitted = split(algoName, '.');
	algoName = splitted.at(0) + "_";
	return algoName;
}

vector<string> AlgorithmFactory::split(const string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

