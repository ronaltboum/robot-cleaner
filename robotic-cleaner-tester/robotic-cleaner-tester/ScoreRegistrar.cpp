#include "ScoreRegistrar.h"
#include <iostream>
#include <dlfcn.h>

using std::cout;
using std::cerr;

// create the static field
ScoreRegistrar ScoreRegistrar::instance;

vector<string> ScoreRegistrar::split(const string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

//returns the score formula folder.  for example:  for input invalidScore/score_formula.so : returns invalidScore.
string ScoreRegistrar::ExtractScoreFolder(string scoreFilePath)
{
	char delim = '/';
	vector<string> nameSplit = split(scoreFilePath, delim);
	int len = nameSplit.size();
	if(len <= 1)
		return scoreFilePath;   //TODO:  handle this case.  Probably can't happen
	string scoreFolder = nameSplit.at(len-2);
	cout << "in ScoreRegistrar::ExtractScoreFolder and scoreFolder = " << scoreFolder << endl;   //delete !!!!
	return scoreFolder;
}


//----------------------------------------------------------------------------
// ScoreRegistrar::loadScoreFunc
// Load the score so library
// return wheter the file was loaded successfully
//----------------------------------------------------------------------------
string ScoreRegistrar::loadScoreFunc(const std::string& folderPath) {
	//_debug = true;  //delete !!!!!!!!!!!!!!!!!!!!!!
	string folderPathCopy = folderPath;
	string suffix = "score_formula.so";
	string fullPath = folderPath;
	bool isNotDefault = false;
	if(folderPath != "./score_formula.so") {
		fullPath = folderPath + suffix;
		isNotDefault = true;
	}
	if(isNotDefault == false)
			folderPathCopy = "./";
	if(_debug) cout << "ScoreRegistrar::loadScoreFunc fullPath: " << fullPath <<endl;

	struct stat buffer;   
	bool exists =  (stat (fullPath.c_str(), &buffer) == 0);
	if(exists == false) {
		string *absolutePath = GetAbsolutePath(folderPathCopy);  //returns null on error, or prints in success
		if(absolutePath == NULL) {  //use relative path
			cout << "cannot find score_formula.so file in '" << folderPath << "'" << endl;
   		}		
		else {
			cout << "cannot find score_formula.so file in '" << (*absolutePath) << "'" << endl;
			delete absolutePath;
		}
		return "cannot find case";
	}

    dl_file = dlopen( fullPath.c_str(), RTLD_LAZY);
    
    if (!dl_file) {
        string *absolutePath = GetAbsolutePath(folderPathCopy);  //returns null on error, or prints in success
		if(absolutePath == NULL) {  //use relative path
			cout << "score_formula.so exists in '" << folderPath << "' but cannot be opened or is not a valid .so" << endl;
   		}		
		else {
			cout << "score_formula.so exists in '"  << (*absolutePath) << "' but cannot be opened or is not a valid .so" << endl;
			delete absolutePath;
		}
		//cout << "score_formula.so exists in '" << fullPath << "' but cannot be opened or is not a valid .so" << endl;
        return "invalid so";
    }
    
    // load the symbol
    if(_debug) cout << "Loading symbol calc_score...\n";
    typedef int (*calc_score_t)(const map<string, int>&);

    // reset errors
    dlerror();

    // calc_score_func = (calc_score_t) dlsym(dl_file, "calc_score");  // not working raising pointer-function error
    // workaround
    static_assert(sizeof(void *) == sizeof(calc_score_t *), "pointer cast impossible");
    void   * vp = dlsym(dl_file, "calc_score");
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
        if(_debug) cout << "cannot load score_func: " << dlsym_error << endl;
        dlclose(dl_file);
    cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
        return "invalid score formula case";
    }
    *reinterpret_cast<void **>(&calc_score_func) = vp;
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
