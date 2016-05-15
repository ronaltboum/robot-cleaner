//#include "stdafx.h"
#include "Direction.h"
#include "AlgorithmFactory.h"
#include "AbstractAlgorithm.h"

#include "Simulator.h"
#include "CommandLineInterpeter.h"
#include "SingletonHouseIOManager.h"
#include "FilesListerWithSuffix.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef std::map<std::string, std::string>::iterator it_type;

int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL));
	
	vector<string> commandLineArguments = CommandLineInterpeter::readCommandLineArguments(argc, argv);
	string configFile = commandLineArguments[0];
	string houseFolder = commandLineArguments[1];
	//string algorithmFolder = (commandLineArguments.size() == 3) ? commandLineArguments[2] : "./";
	string algorithmFolder = commandLineArguments[2];
	
	//Simulator s = Simulator();
	Simulator s;
	bool con = s.ReadConfigFile(configFile); 
	if(con == false)
	    return EXIT_FAILURE;
	
	
	vector<string> algorithmFiles;
	int numAlgoFiles = 0;
	try{
	 FilesListerWithSuffix algorithmsSuffix = FilesListerWithSuffix(algorithmFolder, ".so" );
	 algorithmFiles = algorithmsSuffix.getFilesList();
	 numAlgoFiles = algorithmFiles.size();
	 //case where there are no algorithms in the folder provided by algorithm_path, or there was no algorithm path provided and there are no algorithms in the current directory
	 if (algorithmFiles.size() == 0){
	   throw 20;
	 }
	}
	catch(int e){
	  cout << "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]" << endl;
	  return EXIT_FAILURE;
	}
	
	
	int validAlgo = s.LoadAlgorithms(algorithmFiles);  
	if(validAlgo == 0){
	    //cout << "All algorithm files in target folder '<full path of target folder>' cannot be opened or are invalid:" << endl; 
	    
	    string *absolutePath = s.GetAbsPath(algorithmFolder);
	    if(absolutePath == NULL) {  //use relative path
	      cout << "All algorithm files in target folder '" << algorithmFolder << "' cannot be opened or are invalid:" << endl;
	    }
	    else{
		cout << "All algorithm files in target folder '" << (*absolutePath) << "' cannot be opened or are invalid:" << endl;
		delete absolutePath;
	    }
	    
	    s.PrintAlgoErrors();
	    return EXIT_FAILURE;
	}
	
	vector<string> houseFiles;
	int numHouseFiles = 0;

	try {
	    FilesListerWithSuffix fileListerWithSuffix = FilesListerWithSuffix(houseFolder, ".house" );
	    houseFiles = fileListerWithSuffix.getFilesList();
	    //case where there are no houses in the folder provided by house_path or no house folder was provided and there are no houses in the current directory
	    numHouseFiles = houseFiles.size();
	    if ( numHouseFiles == 0){ 
	      throw 20;
	    }
	}
	catch(int e) {
	    cout << "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]" << endl;
	    return EXIT_FAILURE;
	}
	
	
	int validHouses = s.LoadHouses(houseFiles);
	if(validHouses == 0) {   
	    //cout << "All house files in target folder '<full path of target folder>' cannot be opened or are invalid:" << endl; 
	
	    string *absolutePath = s.GetAbsPath(houseFolder);
	    if(absolutePath == NULL) {  //use relative path
	      cout << "All house files in target folder '" << houseFolder << "' cannot be opened or are invalid:" << endl;
	    }
	    else{
		cout << "All house files in target folder '" << (*absolutePath) << "' cannot be opened or are invalid:" << endl;
		delete absolutePath;
	    }
	
	    s.PrintHouseErrors();  
	    return EXIT_FAILURE;
	}
	
 	s.LoadRuns();
	
	//size_t num_threads = 3;  //TODO:  should get number of threads from command line arguments.  also handle case where num_threads > num of houses
	size_t num_threads = 1;
	
	
 	s.RunAllHouses(num_threads);
	s.printScores();
	
	cout << endl;
	 //(validHouses != 0)
	if( (numHouseFiles > validHouses) || (numAlgoFiles > validAlgo) )
	   cout << "Errors:" << endl;  
	
	s.PrintHouseErrors();
	s.PrintAlgoErrors();
	//getchar();
	return EXIT_SUCCESS;
}


