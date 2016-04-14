#include "AlgorithmFactory.h"
#include "AbstractAlgorithm.h"
#define BUF_SIZE 1024

using namespace std;

// our global factory for making Algorithms
map<string, maker_t *, less<string> > factory;

int AlgorithmFactory::ReadAlgorithms(string algorithmsPath  /*= "./"*/ ){
   FILE *dl;   // handle to read directory
   const char *command_str = "ls *.so";  // command string to get dynamic lib names
   char in_buf[BUF_SIZE]; // input buffer for lib names
   dl_itr dlItr;
   f_itr fitr;
   // get the names of all the dynamic libs (.so  files) in the current dir
   dl = popen(command_str, "r");
   if(!dl){
	  perror("popen");
	  exit(-1);
   }
   void *dlib;
   char name[1024];
   while(fgets(in_buf, BUF_SIZE, dl)){
	  // trim off the whitespace
	  char *ws = strpbrk(in_buf, " \t\n");
	  if(ws) *ws = '\0';
	  // append ./ to the front of the lib name
	  sprintf(name, "./%s", in_buf);
	  dlib = dlopen(name, RTLD_NOW);
	  if(dlib == NULL){
		 cerr << dlerror() << endl;
		 exit(-1);
	  }
	  // add the handle to our list
	  dl_list.insert(dl_list.end(), dlib);
   }
   int i = 0;
   // create an array of the AbstractAlgorithm names
   for(fitr=factory.begin(); fitr!=factory.end(); fitr++){
	  algorithmNames.insert(algorithmNames.end(), fitr->first);
	  i++;
   }
   return i;
}

void AlgorithmFactory::tryAlgorithms()
{
	vector<AbstractAlgorithm *> algoSet = CreateSetOfAllAlgorithms();
	vector<AbstractAlgorithm *>::iterator aItr;
	for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
		(*aItr)->setConfiguration(map<string, int>());
		Direction d = (*aItr)->step();
		(*aItr)->aboutToFinish(0);
	}
	
	for(aItr=algoSet.begin(); aItr!=algoSet.end();aItr++){
		delete *aItr;
	}
}


vector<AbstractAlgorithm *> AlgorithmFactory::CreateSetOfAllAlgorithms()
{
	vector<AbstractAlgorithm *> algorithmSet = vector<AbstractAlgorithm *>();
	vector<string>::iterator nItr;
	for(nItr=algorithmNames.begin(); nItr!=algorithmNames.end(); nItr++){
		algorithmSet.push_back(factory[*nItr]());
	}
	return algorithmSet;
}
