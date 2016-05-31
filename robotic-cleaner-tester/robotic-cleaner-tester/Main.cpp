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
#define printUsageMessage() cout << "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]" << endl

int main(int argc, const char * argv[])
{
	CommandLineInterpeter::CommandLineProblems problemOccored =
	 	CommandLineInterpeter::readCommandLineArguments(argc, argv);
	switch(problemOccored){
		case CommandLineInterpeter::CommandLineProblems::ThreadIsntNum:
		case CommandLineInterpeter::CommandLineProblems::OddCLA:
		case CommandLineInterpeter::CommandLineProblems::UnknownCLA:
			printUsageMessage();
			return EXIT_FAILURE;
		default:
			break;
	}

	
	string configFile = CommandLineInterpeter::getConfigFile();
	if(configFile != "config.ini") {
		int len = configFile.length();
		if(configFile.at(len - 1) != '/')
			configFile = configFile + "/";

	}
	//cout << "after configFile = " << configFile << endl;  //delete !!!!

	const string houseFolder = CommandLineInterpeter::getHousePath();
	const string algorithmFolder = CommandLineInterpeter::getAlgorithmPath();
	const string scoreFormulaFile = CommandLineInterpeter::getScoreFormulaFile();
	int num_threads = CommandLineInterpeter::getThreads(); //TODO:  test several algos together with printings to make sure MoveAllOneStep works ok
	if(num_threads == -1){
		//shouldn't get here
		printUsageMessage();
		return EXIT_FAILURE;
	}

	
	//reading config file
	Simulator s;
	bool con = s.ReadConfigFile(configFile); 
	if(con == false)
	    return EXIT_FAILURE;
	
	// loading score
	string errorLoadingScore = ScoreRegistrar::getInstance().loadScoreFunc(scoreFormulaFile);
	if(errorLoadingScore != "" ){
		//cout << errorLoadingScore;
		return EXIT_FAILURE;
	}

	
	vector<string> algorithmFiles;
	int numAlgoFiles = 0;
	try{
		FilesListerWithSuffix algorithmsSuffix = FilesListerWithSuffix(algorithmFolder, ".so" );
		algorithmFiles = algorithmsSuffix.getFilesList();
		
		//removing score formula file
		algorithmFiles.erase(std::remove_if(algorithmFiles.begin(), 
                              algorithmFiles.end(),
                              [](string filename){return (filename.find("score_formula.so") != string::npos );}),
               algorithmFiles.end());
		numAlgoFiles = algorithmFiles.size();
		//case where there are no algorithms in the folder provided by algorithm_path, or there was no algorithm path provided and there are no algorithms in the current directory
		if (algorithmFiles.size() == 0){
			throw 20;
		}
	}
	catch(int e){
		printUsageMessage();
		string *absolutePath = s.GetAbsPath(algorithmFolder);
	    if(absolutePath == NULL) {
	      cout << "​cannot find algorithm files in '" << algorithmFolder << "'" << endl;
	    }
	    else{
		cout << "​cannot find algorithm files in '" << (*absolutePath) << "'" << endl;
		delete absolutePath;
	    }
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
	catch(int e){
		printUsageMessage();
		string *absolutePath = s.GetAbsPath(houseFolder);
	    if(absolutePath == NULL) {
	      cout << "​cannot find house files in '" << houseFolder << "'" << endl;
	    }
	    else{
			cout << "​cannot find house files in '" << (*absolutePath) << "'" << endl;
			delete absolutePath;
	    }
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
	
 	s.RunAllHouses(num_threads);
	s.printScores();
	
	cout << endl;
	 //(validHouses != 0)
	if( (numHouseFiles > validHouses) || (numAlgoFiles > validAlgo) || s.GetScoreFormulaProblem() == true )
	   cout << "Errors:" << endl;  
		s.PrintHouseErrors();
		s.PrintAlgoErrors();
		if(s.GetScoreFormulaProblem() == true)
			cout << "Score formula could not calculate some scores, see ­1 in the results table" << endl;
		//getchar();
		return EXIT_SUCCESS;
}


