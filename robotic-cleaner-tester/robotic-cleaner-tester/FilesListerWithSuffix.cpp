#include "FilesListerWithSuffix.h"


 FilesListerWithSuffix::FilesListerWithSuffix(const string& basePath, const string& suffix)
    :  FilesLister::FilesLister(basePath)
    ,  suffix_ (suffix)
    {
      this->filterFiles();
    }
  
  
 void FilesListerWithSuffix::filterFiles()
  {
    vector<string> temp = this->filesList_;
    this->filesList_.clear();
    for (vector<string>::iterator itr = temp.begin();
         itr != temp.end();
         ++itr)
    {
      if (endsWith(*itr, this->suffix_))
      {
        this->filesList_.push_back(*itr);
      }
    }
  } 
  
  
