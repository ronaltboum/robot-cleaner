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
    void *dlib;
    size_t size = instance.size();
    string errorMessage;
    cout << "opening: " << fullPath << endl;
    dlib = dlopen( fullPath.c_str(), RTLD_NOW);
    if(dlib == NULL){
        errorMessage = "file cannot be loaded or is not a valid .so";
        _badAlgoMap.insert( pair<string, string>(algoName, errorMessage) );
        return false;
    }
    if(instance.size() == size) {
        dlclose(dlib);
        errorMessage = ".so was loaded but no algorithm was registered";
        _badAlgoMap.insert( pair<string, string>(algoName, errorMessage) );
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
        cuttedString = cuttedString.substr(idx);
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
