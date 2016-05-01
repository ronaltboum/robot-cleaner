#ifndef __FilesListerWithSuffix__H_
#define __FilesListerWithSuffix__H_

#include "FilesLister.h"
using namespace std;


  class FilesListerWithSuffix : public FilesLister
  {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  protected:
    string suffix_;
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    
  public:
    FilesListerWithSuffix(const string& basePath, const string& suffix);
 
    
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~       
  public:
//     virtual void refresh()
//      {
//       FilesLister::refresh();
//       this->filterFiles();
//      }
  
  
  protected:
    void filterFiles();
    
    static inline bool endsWith(std::string value, std::string ending)
    {
      if (ending.size() > value.size()) return false;
      return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    }
    

  };
  

#endif //__FilesListerWithSuffix__H_