#ifndef HouseFileReader_h__
#define HouseFileReader_h__

#include <string.h>
#include <vector>
#include <map>
#include <sstream> 
#include <fstream>
#include "House.h"
#include "Matrix.h"

using namespace std;

class House;

class SingletonHouseIOManager
{
	friend class House;
	//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
	#define houseDefaultFolder "./"
	#define houseFilePostfix ".house"
	#define wallString string(1, d_cWallLetter)

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		string _houseFolder; // the folder path of the house files
		static SingletonHouseIOManager * s_instance; // singleton instance
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	private:
		SingletonHouseIOManager(const string & houseFolder);
		SingletonHouseIOManager();
		~SingletonHouseIOManager(void);
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		string GetFolder();
		void SetFolder(string houseFolder);
		static SingletonHouseIOManager *instance() ; //get instance of the singleton
		static SingletonHouseIOManager *instance(const string & houseFolder); //get instance of the singleton and set foldername

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	public:
		vector<House *> ReadHouses();
		vector<House *> ReadHouses(const string & houseFolder);
		static House * ReadHouseFromFile(string houseFileName);
		static House * ReadHouseFromFile(istream & in);
		static string ExtractHouseName(string houseFileName); //returns the house name without the suffix.  for example:  for input dir/simple1.house, returns simple1
private:
		static void processRowFromFile(string & rowFromFile, int currentRowNumber, House & h);
		static void fillRow(string & rowReadFromFile, int width);
		static void fillLastRows(int rowsToFill, House & h);
		static void ValidateWallsAndCharacters(House & h);  //: check that all characters are valid, and change them if not. puts walls in the sides.
		static vector<string> split(const string &s, char delim);  //splits a line by delim
		
};

inline string SingletonHouseIOManager::GetFolder()
{
	return _houseFolder;
}

inline void SingletonHouseIOManager::SetFolder(string houseFolder)
{
	_houseFolder = houseFolder;
}

inline SingletonHouseIOManager *  SingletonHouseIOManager::instance()
{
	if (! s_instance)
		s_instance = new SingletonHouseIOManager();
	return s_instance;
}

inline 	SingletonHouseIOManager * SingletonHouseIOManager::instance(const string & houseFolder)
{
	SingletonHouseIOManager * thisInstance = instance();
	thisInstance->SetFolder(houseFolder);
	return thisInstance;
}

#endif // HouseFileReader_h__

