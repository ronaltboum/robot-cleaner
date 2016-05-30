#ifndef ConfigReader_h__
#define ConfigReader_h__

#include <string>
#include <vector>
#include <map>
#include <sstream> 
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <limits.h> /* PATH_MAX */
#include <cstring>

using namespace std;

/*
 * class SingletonConfigReader is a singleton class for reading the config file
 * it's main method is ReadConfigFromFile() which 
 * returns a map of configuration names mapped to their values
 * 
 */
class SingletonConfigReader
{
//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
	typedef std::map<string, int> StringToIntMap;
	typedef std::pair<string, int> StringIntPair;
	#define configFileName "config.ini"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	string _configFilePath;	// the file path of the configuration file
	static const StringToIntMap defaultValues; // a static map of config names and their default values
	static StringToIntMap configMap;  //the map that contains values from config file
	static SingletonConfigReader * s_instance; // singleton instance
	//bool _existBadValues = false;  //becomes true if there are bad values for the paramters
	static vector<string> badValues;

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
	static map<string,int> GetConfigMap();
	static string* GetAbsolutePath(string relativePath);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	bool ReadConfigFromFile(); //reads the config file
	bool IsConfigValid ();  //checks if config file is valid

private:
	static string trim(string& str); //used to trim string of their white spaces
	static void processLine(const string & line); // processing one line of the input
	static vector<string> split(const string & line, char delim); //splitting line according to the delimiter delim
	static StringToIntMap initDefaultValues(); // initiallize the map of default values
	static StringToIntMap initConfigMap();
	static vector<string> initBadValuesVector();
	bool HandleMissingParameters();
	void PrintMissingParameters(vector<string> missingParameters);
	void PrintBadValues();
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
	//myMap["MaxSteps"] = 1200;
	myMap["MaxStepsAfterWinner"] = 200;
	myMap["BatteryCapacity"] = 400;
	myMap["BatteryConsumptionRate"] = 1;
	myMap["BatteryRechargeRate"] = 20;
	return myMap;
}

inline SingletonConfigReader::StringToIntMap SingletonConfigReader::initConfigMap(){
	StringToIntMap configMap = map<string,int>();
	return configMap;
}

inline map<string,int> SingletonConfigReader::GetConfigMap()
{
  return configMap;
}

inline vector<string> SingletonConfigReader::initBadValuesVector(){
	vector<string> badValues = vector<string>();
	return badValues;
}


#endif // ConfigReader_h__
