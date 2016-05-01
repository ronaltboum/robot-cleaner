#ifndef __FilesLister__H_
#define __FilesLister__H_

#include <dirent.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


  class FilesLister 
  { 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  protected:
  vector<string> filesList_;
  string basePath_;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  public:
  FilesLister(const string& basePath);
  
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
  vector<string> getFilesList();
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  virtual void refresh();
  void printStringVector (const vector<string>& vec);
  
  private:
    static string concatenateAbsolutePath(const string& dirPath, const string& fileName);
 
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~
inline vector<string> FilesLister::getFilesList()
{
  return this->filesList_;
}


#endif //__FilesLister__H_


