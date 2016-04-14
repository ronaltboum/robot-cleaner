 #include "AlgorithmFactory.h"
#include "AbstractAlgorithm.h"

#include "Simulator.h"
#include "CommandLineInterpeter.h"
#include "SingletonHouseIOManager.h"
#include "DynamicHouse.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	srand((unsigned int)time(NULL));
	
	vector<string> commandLineArguments = CommandLineInterpeter::readCommandLineArguments(argc, argv);
	cout << "started1" << endl;
	string configFile = commandLineArguments[0];
	string houseFolder = commandLineArguments[1];
	string algorithmFolder = (commandLineArguments.size() == 3) ? commandLineArguments[2] : "./";
	//algorithmFolder = commandLineArguments[2];

	////print for debugging.  delete later !!!!!!!!!!!!
	//for(vector<string>::const_iterator i = commandLineArguments.begin(); i != commandLineArguments.end(); ++i) {
 //   // process i
 //   cout << *i << " "; // this will print all the contents of *features*
	//}

	
	Simulator s = Simulator();
	s.ReadConfigFile(configFile);
	if(s.LoadHouses(houseFolder) == 0){
		cout << "Cannot load house properly. exiting";
		return EXIT_FAILURE;
	}
	
	s.LoadAlgorithms(algorithmFolder);
	cout << "loaded algo and tried them" << endl;
	s.LoadRuns();
	s.RunAll();
	getchar();
	return EXIT_SUCCESS;
}


