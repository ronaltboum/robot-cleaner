#include "FilesLister.h"
using namespace std;


  FilesLister::FilesLister(const string& basePath)
  :  basePath_ (basePath)
  {
    this->refresh();
  }

  void FilesLister::refresh()
  {
    DIR *dir;
    struct dirent *ent;
    this->filesList_.clear();
    
    if(this->basePath_ == "./"){  //case where we have to search the current directory
      if( (dir = opendir(".")) != NULL){
	while ((ent = readdir (dir)) != NULL) {
	  this->filesList_.push_back(FilesLister::concatenateAbsolutePath("", ent->d_name));
      }
      closedir (dir);
      }
      
    }
    
    else if ((dir = opendir (this->basePath_.c_str())) != NULL) {
      
      while ((ent = readdir (dir)) != NULL) {
        this->filesList_.push_back(FilesLister::concatenateAbsolutePath(this->basePath_, ent->d_name));
      }
      closedir (dir);
    } else {
    
    //cout << "Error: could not open directory: " <<  this->basePath_ << endl;  // delete !!!!!!!!!!!!!!!!!!!!!!!!!!
     
      throw 20;
      return;
    }
    std::sort(this->filesList_.begin(), this->filesList_.end());
  }


  
  string FilesLister::concatenateAbsolutePath(const string& dirPath, const string& fileName)
  {
    if (dirPath.empty()){
      //cout << "i'm in dirPath.empty" << endl;  //delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      return fileName;
      
    }
    
//     else if (dirPath.back() == "./"){
//       return fileName;
//     }
    
    if( dirPath[dirPath.length() - 1] == '/' )
      return dirPath  + fileName;
    
    //if (dirPath.back() == '/')
      //return dirPath  + fileName;
    return dirPath + "/" + fileName;
  }


  
void FilesLister::printStringVector (const vector<string>& vec)
{
  for (vector<string>::const_iterator itr = vec.begin();
       itr != vec.end();
       ++itr)
  {
    cout << *itr;
    if (itr+1 != vec.end())
    {
      cout << ", ";
    }
  }
  cout << endl;
}


// int main(int argc, char** argv)
// {
//   string basePath = "";
//   if (argc > 1)
//     basePath = argv[0];
// 
//   basePath = "dir";
//   system(("mkdir " + basePath).c_str());
//   for (int i = 0; i < 100; ++i)
//   {
//     fclose(fopen((basePath+ "/" + "house" + std::to_string(i) + ".house").c_str(), "w"));
//     fclose(fopen((basePath+ "/" + "algorithm" + std::to_string(i) + ".so").c_str(), "w"));
//     fclose(fopen((basePath+ "/" + "junk" + std::to_string(i) + ".junk").c_str(), "w"));
//   }
// 
//   HousesLister housesLister = HousesLister(basePath);
//   AlgorithmsLister algorithmsLister = AlgorithmsLister(basePath);
//   cout << "Houses: " << endl;
//   printStringVector(housesLister.getFilesList());
//   cout << endl;
//   cout << "Algorithms: " << endl;
//   printStringVector(algorithmsLister.getFilesList());
//   return 0;
// }

