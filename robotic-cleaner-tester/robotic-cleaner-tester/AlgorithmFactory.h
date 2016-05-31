#ifndef __ALGORITHMFACTORY__H_
#define __ALGORITHMFACTORY__H_

#include <iostream>
#include <functional>
#include <map>
#include <memory>
#include <list>
#include <vector>
#include <cassert>
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "Direction.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
#include "MakeUnique.h"
#include "AlgorithmRegistration.h"

using namespace std;

//========================================================================
// AlgorithmFactory.h is part of the simulation project ONLY
// You have the freedom to implement it as you wish!
// You even don't have to have such a class!
//========================================================================
class AlgorithmFactory {
private:
    list<void *> dl_list; // list to hold handles for dynamic libs
    vector<string> algorithmNames; 
    map<string, string> _badAlgoMap;
    list<std::function<unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    static AlgorithmFactory instance;

    AlgorithmFactory(){};
    void registerAlgorithm(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
        instance.algorithmFactories.push_back(algorithmFactory);
    }
    void setNameForLastAlgorithm(const std::string& algorithmName) {
        assert(algorithmFactories.size()-1 == algorithmNames.size());
        algorithmNames.push_back(algorithmName);
    	// cout << "AlgorithmFactory::setNameForLastAlgorithm " << algorithmName << " was added" << endl;
    }

    bool loadAlgorithm(const string& fullPath, const string& algoName);
public:
    friend class AlgorithmRegistration;
    vector<AbstractAlgorithm *> CreateSetOfAllAlgorithms() const;
    const vector<string>& GetAlgorithmNames() const {
        return algorithmNames;
    }
    size_t size()const {
        return algorithmFactories.size();
    }
    static AlgorithmFactory& getInstance() {
        return instance;
    }
    // ~~~~~~~~~~~~~~~from factory
    void tryAlgorithms();
    int ReadAlgorithms(vector<string> algorithmFiles); 
    
    map<string,string> GetBadAlgoMap() { return _badAlgoMap;}
    //void SetBadAlgoMap(map<string,string> errorMap);
    string ExtractFileName (string toSplit);
    string GetRidOfSuffix (string fileName);
private:

};

#endif //__ALGORITHMFACTORY__H_
