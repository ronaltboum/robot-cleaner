#ifndef ConfigReader_h__
#define ConfigReader_h__

#include <string>
#include <vector>
#include <map>
#include <sstream> 
#include <fstream>
using namespace std;

namespace ns_robotic_cleaner_simulator
{

/*
 * class SingletonConfigReader is a singleton class for reading the config file
 * it's main method is ReadConfigFromFile() which 
 * returns a map of configuration names mapped to their values
 * in order to 
 */
class SingletonConfigReader
{
//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
	typedef std::map<string, int> StringToIntMap;
	#define configFileName "config.ini"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	string _configFilePath;	// the file path of the configuration file
	static const StringToIntMap defaultValues; // a static map of config names and their default values
	static SingletonConfigReader *s_instance; // singleton instance

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	SingletonConfigReader(const string & configFilePath);
	SingletonConfigReader(){}
	~SingletonConfigReader(void);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	string GetFilePath(); 
	void SetFilePath(string configFilePath);
	static SingletonConfigReader * instance(); //get instance of the singleton
	static SingletonConfigReader * instance(const string & configFilePath); //get instance of the singleton and set filename

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	map<string, int> ReadConfigFromFile(); //reads the config file and returns them as a map

private:
	static string trim(string& str); //used to trim string of their white spaces
	static bool processLine(const string & line, map<string, int> & configMap); // processing one line of the input
	static vector<string> split(const string & line, char delim); //splitting line according to the delimiter delim
	static StringToIntMap initDefaultValues(); // initiallize the map of default values
	void CompleteMissingConfigs(map<string, int> & configs); // gets config values, prints warning about missing configs, add default values
};

inline string SingletonConfigReader::GetFilePath()
{
	return _configFilePath;
}

inline void SingletonConfigReader::SetFilePath(string configFilePath)
{
	_configFilePath = configFilePath;
}

inline SingletonConfigReader *  SingletonConfigReader::instance()
{
	if (!s_instance)
		s_instance = new SingletonConfigReader();
	return s_instance;
}

inline 	SingletonConfigReader * SingletonConfigReader::instance(const string & configFilePath)
{
	SingletonConfigReader * thisInstance = instance();
	thisInstance->SetFilePath(configFilePath);
	return thisInstance;
}

inline SingletonConfigReader::StringToIntMap SingletonConfigReader::initDefaultValues(){
	StringToIntMap myMap;
	myMap["MaxSteps"] = 1200;
	myMap["MaxStepsAfterWinner"] = 200;
	myMap["BatteryCapacity"] = 400;
	myMap["BatteryConsumptionRate"] = 1;
	myMap["BatteryRechargeRate"] = 20;
	return myMap;
}
	
} //end of namespace ns_robotic_cleaner_simulator

#endif // ConfigReader_h__
