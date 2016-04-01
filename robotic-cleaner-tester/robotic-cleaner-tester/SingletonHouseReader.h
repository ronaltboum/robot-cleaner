#ifndef HouseFileReader_h__
#define HouseFileReader_h__

#include <string.h>
#include <vector>
#include <map>
#include <sstream> 
#include <fstream>
#include "House.h"

using namespace std;

namespace ns_robotic_cleaner_simulator
{


class SingletonHouseReader
{

	//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
	#define houseDefaultFolder "./"
	#define houseFilePostfix ".house"

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		string _houseFolder; // the folder path of the house files
		static SingletonHouseReader *s_instance; // singleton instance

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		SingletonHouseReader(const string & houseFolder);
		SingletonHouseReader();
		~SingletonHouseReader(void);
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		string GetFolder();
		void SetFolder(string houseFolder);
		static SingletonHouseReader *instance() ; //get instance of the singleton
		static SingletonHouseReader *instance(const string & houseFolder); //get instance of the singleton and set foldername

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		vector<House *> ReadHouses();
		vector<House *> ReadHouses(const string & houseFolder);
		House * ReadHouseFromFile(string houseFileName);
private:
		//static string trim(string& str);
		//static bool processLine(const string & line, map<string, int> & configMap);
		//static vector<string> split(const string &s, char delim);
	};

inline string SingletonHouseReader::GetFolder()
{
	return _houseFolder;
}

inline void SingletonHouseReader::SetFolder(string houseFolder)
{
	_houseFolder = houseFolder;
}

inline SingletonHouseReader *  SingletonHouseReader::instance()
{
	if (!s_instance)
		s_instance = new SingletonHouseReader();
	return s_instance;
}

inline 	SingletonHouseReader * SingletonHouseReader::instance(const string & houseFolder)
{
	SingletonHouseReader * thisInstance = instance();
	thisInstance->SetFolder(houseFolder);
	return thisInstance;
}


} //end of namespace ns_robotic_cleaner_simulator

#endif // HouseFileReader_h__

