#include "ScoreRegistrar.h"
#include <iostream>
#include <dlfcn.h>

using std::cout;
using std::cerr;

// create the static field
ScoreRegistrar ScoreRegistrar::instance;

//----------------------------------------------------------------------------
// ScoreRegistrar::loadScoreFunc
// Load the score so library
// return wheter the file was loaded successfully
//----------------------------------------------------------------------------
string ScoreRegistrar::loadScoreFunc(const std::string& fullPath) {
    if(_debug) cout << "ScoreRegistrar::loadScoreFunc fullPath: " << fullPath <<endl;

    // open the library
    //cout << "opening: " << fullPath << endl; 

	struct stat buffer;   
	bool exists =  (stat (fullPath.c_str(), &buffer) == 0);
	if(exists == false) {
		//cout << "i'm here" << endl; //delete !!!!
		cout << "cannot find score_formula.so file in '" << fullPath << "'" << endl;
		return "cannot find case";
	}

    dl_file = dlopen( fullPath.c_str(), RTLD_LAZY);
    
    if (!dl_file) {
        //cout << NOT_VALID_SO << endl;
		//cout << "in ScoreRegistrar::loadScoreFunc in case NOT_VALID_SO" << endl; //delete !!!!!!!
		cout << "score_formula.so exists in '" << fullPath << "' but cannot be opened or is not a valid .so" << endl;
        return "invalid so";
    }
    
    // load the symbol
    if(_debug) cout << "Loading symbol calc_score...\n";
    typedef int (*calc_score_t)(const map<string, int>&);

    // reset errors
    dlerror();
    calc_score_func = (calc_score_t) dlsym(dl_file, "calc_score");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        if(_debug) cout << "cannot load score_func: " << dlsym_error << endl;
        dlclose(dl_file);
	cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
        return "invalid score formula case";
    }
    return "";
}

string ScoreRegistrar::ExtractFileName (string filename)
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

string ScoreRegistrar::GetRidOfSuffix (string filename)
{

    size_t idx = filename.rfind('.');
    if(idx != std::string::npos)
    {
        return filename.substr(0, idx);
    }
    return filename;
}

string* ScoreRegistrar::GetAbsolutePath(string relativePath)
{
  
  char * relative = new char [relativePath.length()+1];
  std::strcpy (relative, relativePath.c_str());
  char actualpath [PATH_MAX + 1];
  char *ptr;
  ptr = realpath(relative, actualpath); 
  delete[] relative;
  std::string* abs = NULL;
  if(ptr != NULL){
    abs = new string(actualpath);  //calling function needs to delete abs in case it was allocateds
  }
  return abs;
}
