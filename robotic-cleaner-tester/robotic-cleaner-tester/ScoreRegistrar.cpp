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
    cout << "opening: " << fullPath << endl;
    dl_file = dlopen( fullPath.c_str(), RTLD_LAZY);
    
    if (!dl_file) {
        cout << NOT_VALID_SO << endl;
        return NOT_VALID_SO;
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
        return NO_SCORE_FUNCTION;
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