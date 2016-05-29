#ifndef __SCOREREGISTRAR__H_
#define __SCOREREGISTRAR__H_

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

using namespace std;

//========================================================================
// ScoreRegistrar.h is part of the simulation project ONLY
// You have the freedom to implement it as you wish!
// You even don't have to have such a class!
//========================================================================
class ScoreRegistrar {
private:
	typedef int (*calc_score_t)(const map<string, int>&);
	#define SCORE_CANNOT_BE_CALCULATED -2
    #define NOT_VALID_SO "score_formula.so exists in '<full path>' but cannot be opened or is not a valid .so"
    #define NO_SCORE_FUNCTION "score_formula.so is a valid .so but it does not have a valid score formula";
    void * dl_file; // list to hold handles for dynamic libs
    calc_score_t calc_score_func;
    bool score_func_loaded;
    bool _debug = false;
    static ScoreRegistrar instance;
    ScoreRegistrar(){
    	score_func_loaded = false;
    }
public:
	static ScoreRegistrar& getInstance() {
        return instance;
    }
	string loadScoreFunc(const string& fullPath);
	~ScoreRegistrar(){
		if (dl_file)
        	dlclose(dl_file);
	}
	int calc_score(const map<string, int>& params){
		try 
		{  
			if(calc_score_func && dl_file)
				return calc_score_func(params);
			else
				return SCORE_CANNOT_BE_CALCULATED;
		}
		catch(const std::exception& e)
		{
			return SCORE_CANNOT_BE_CALCULATED;
		}
	}

    string ExtractFileName (string toSplit);
    string GetRidOfSuffix (string fileName);

};

#endif //__SCOREREGISTRAR__H_
