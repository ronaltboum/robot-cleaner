#ifndef CommandLineInterpeter_h__
#define CommandLineInterpeter_h__

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace ns_robotic_cleaner_simulator
{

/*
 * class CommandLineInterpeter is a "static" class with one function - 
 *	readCommandLineArguments for handling command line arguments.
 */
class CommandLineInterpeter
{
//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
#define configFileName "config.ini"
#define configFlag "-config"
#define housePathFlag "-house_path"
#define algorithmPathFlag "-algorithm_path"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	CommandLineInterpeter(void){}
	~CommandLineInterpeter(void){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	static vector<string> readCommandLineArguments(int argc, char * argv[]){
		string configFilePath;
		string houseFolder;
		string algorithmFolder;

		if(argc >= 3) //case where there is at least one argument given by the user
		{
			if ( ! strcmp(argv[1], configFlag))
			{
				configFilePath = string(argv[2]);
			}
			else if (! strcmp(argv[1], housePathFlag))
			{
				houseFolder = string(argv[2]);
			}
			else if (! strcmp(argv[1], algorithmPathFlag))
			{
				algorithmFolder = string(argv[2]);
			}
			else
			{
				cout << "invalid flags. flags are -config and -house_path and -algorithm_path";
			}
		}
		if(argc >= 5)  //case where only 2 out of 3 arguments were given by the user
		{
			if ( ! strcmp(argv[3], configFlag))
			{
				configFilePath = string(argv[4]);
			}
			else if (! strcmp(argv[3], housePathFlag))
			{
				houseFolder = string(argv[4]);
			}
			else if (! strcmp(argv[3], algorithmPathFlag))
			{
				algorithmFolder = string(argv[4]);
			}
			else
			{
				cout << "invalid flags. flags are -config and -house_path and -algorithm_path";
			}
		}

		if(argc == 7)  //case where all arguments were given by the user
		{
			
			if ( ! strcmp(argv[5], configFlag))
			{
				configFilePath = string(argv[6]);
			}
			else if (! strcmp(argv[5], housePathFlag))
			{
				houseFolder = string(argv[6]);
			}
			else if (! strcmp(argv[5], algorithmPathFlag))
			{
				algorithmFolder = string(argv[6]);
			}
			else
			{
				cout << "invalid flags. flags are -config and -house_path and -algorithm_path";
			}
		}


		if(configFilePath != ""){
			if( configFilePath[configFilePath.length() - 1] != '/' ){
				configFilePath.append("/");
			}
		}
		configFilePath.append(configFileName);

		if(houseFolder != ""){
			if( houseFolder[houseFolder.length() - 1] != '/' ){
				houseFolder.append("/");
			}
		}

		if(algorithmFolder != ""){
			if( algorithmFolder[algorithmFolder.length() - 1] != '/' ){
				algorithmFolder.append("/");
			}
		}

		vector<string> commandLineArguments = vector<string>();
		commandLineArguments.push_back(configFilePath);
		commandLineArguments.push_back(houseFolder);
		commandLineArguments.push_back(algorithmFolder);
		
		return commandLineArguments;
	}

};
	
} //end of namespace ns_robotic_cleaner_simulator

#endif // CommandLineInterpeter_h__
