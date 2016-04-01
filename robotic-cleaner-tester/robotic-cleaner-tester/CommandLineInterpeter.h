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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	CommandLineInterpeter(void){}
	~CommandLineInterpeter(void){}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	static vector<string> readCommandLineArguments(int argc, char * argv[]){
		string configFilePath;
		string houseFolder;
		if(argc >= 3) 
		{
			if ( ! strcmp(argv[1], configFlag))
			{
				configFilePath = string(argv[2]);
			}
			else if (! strcmp(argv[1], housePathFlag))
			{
				houseFolder = string(argv[2]);
			}
			else
			{
				cout << "invalid flags. flags are -config and -house_path";
			}
		}
		if(argc == 5) 
		{
			if ( ! strcmp(argv[3], configFlag))
			{
				configFilePath = string(argv[4]);
			}
			else if (! strcmp(argv[3], housePathFlag))
			{
				houseFolder = string(argv[4]);
			}
			else
			{
				cout << "invalid flags. flags are -config and -house_path";
			}
		}
		if(configFilePath != ""){
			configFilePath.append("/");
		}
		configFilePath.append(configFileName);
		vector<string> commandLineArguments = vector<string>();
		commandLineArguments.push_back(configFilePath);
		commandLineArguments.push_back(houseFolder);
		return commandLineArguments;
	}

};
	
} //end of namespace ns_robotic_cleaner_simulator

#endif // CommandLineInterpeter_h__
