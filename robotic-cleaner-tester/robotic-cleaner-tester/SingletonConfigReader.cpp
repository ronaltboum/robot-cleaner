//#include "stdafx.h"
#include "SingletonConfigReader.h"
#include <iosfwd>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

SingletonConfigReader *SingletonConfigReader::s_instance = 0;
const SingletonConfigReader::StringToIntMap SingletonConfigReader::defaultValues = initDefaultValues();
SingletonConfigReader::StringToIntMap SingletonConfigReader::configMap = initConfigMap();
vector<string> SingletonConfigReader::badValues = initBadValuesVector();

SingletonConfigReader::SingletonConfigReader(const string & configFilePath)
{
	_configFilePath = configFilePath;
}


SingletonConfigReader::~SingletonConfigReader(void)
{
}

std::vector<std::string> SingletonConfigReader::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::string SingletonConfigReader::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
	return str;
}

//bool SingletonConfigReader::processLine(const string& line, map<string, int> & configMap)
void SingletonConfigReader::processLine(const string& line)
{
	vector<string> tokens = split(line, '=');
	if (tokens.size() != 2)
	{
		return;
	}
	int parameterValue = 0;
	if ( ! (istringstream(tokens[1]) >> parameterValue) ) {
		//_existBadValues = true;
		badValues.push_back(trim(tokens[0]));
		//cout << "in SingletonConfigReader::processLine and can't parse paramter" << endl;
		return;

	}
	
	configMap[trim(tokens[0])] = parameterValue;
	if(parameterValue <= 0)
		badValues.push_back(trim(tokens[0]));
}

//vector<string> SingletonConfigReader::CheckMissingParameters(map<string, int> & configs)
bool SingletonConfigReader::HandleMissingParameters()
{
	vector<string> missingParameters = vector<string>();
	for (StringToIntMap::const_iterator nameValueIterator = defaultValues.begin();
		nameValueIterator != defaultValues.end();
		++nameValueIterator) 
	{
		if ( configMap.find(nameValueIterator->first) == configMap.end() ) {
		      missingParameters.push_back(nameValueIterator->first);
		}
	}
	
	int numMissing = missingParameters.size();
	if( numMissing != 0) {  //there are missing parameters
	    PrintMissingParameters(missingParameters);
	    return false;
	}
	return true;	
}

//map<string, int> SingletonConfigReader::ReadConfigFromFile()
bool SingletonConfigReader::ReadConfigFromFile()
{
	string line;
	ifstream myfile(_configFilePath.c_str());
	//int lineNumber = 1;
	//map<string, int> configs = map<string,int>();
	myfile.is_open();  //we already know config file exists and is valid for reading
	
	while ( getline (myfile,line) )
	{
	    //processLine(line, configs);
	    processLine(line);
	}
	myfile.close();

	if(badValues.size() > 0){
		PrintBadValues();
		return false;
	}
			
	return HandleMissingParameters();
}
	

void SingletonConfigReader::PrintMissingParameters(vector<string> missingParameters)
{
  int size = missingParameters.size();
  cout << "config.ini missing " << size << " parameter(s): ";
  vector<string>::iterator strIt;
  strIt = missingParameters.begin();
  cout << (*strIt);
  if(size == 1){
      cout << endl;
      return;
  }
  ++strIt;  //TODO:  should i advance here?
  for( ; strIt != missingParameters.end();  strIt++) {
      cout << ", " << (*strIt);
  }
  cout << endl;
}

void SingletonConfigReader::PrintBadValues()
{
	//example: config.ini having bad values for 1 parameter(s): BatteryCapacity
	int size = badValues.size();
	if(size == 0)
		return;  //shouldn't get here
	cout << "config.ini having bad values for " << size << " parameter(s): ";
	vector<string>::iterator strIt;
	strIt = badValues.begin();
	cout << (*strIt);
	if(size == 1){
	  cout << endl;
	  return;
	}
	++strIt;  //TODO:  should i advance here?
	for( ; strIt != badValues.end();  strIt++) {
	  cout << ", " << (*strIt);
	}
	cout << endl;
}


bool SingletonConfigReader::IsConfigValid ()
{

  //cout << "in SingletonConfigReader::IsConfigValid and _configFilePath before = " << _configFilePath << endl; //delet

	string folder = _configFilePath;
	if(_configFilePath != "config.ini")
		_configFilePath = _configFilePath + "config.ini";

	//cout << "in SingletonConfigReader::IsConfigValid and _configFilePath after = " << _configFilePath << endl; //delet

  struct stat buffer;   
  bool exists =  (stat (_configFilePath.c_str(), &buffer) == 0);
  if(exists == false) {
    cout << "Usage: simulator [-config <config path>] [-house_path <house path>] [-algorithm_path <algorithm path>]" << endl;
		string *absolutePath = GetAbsolutePath(folder);  //returns null on error, or prints in success
		if(absolutePath == NULL) {  //use relative path
	  	cout << "cannot find config.ini file in '" << _configFilePath << "'" << endl;
    }
     
	  //cout << "config.ini exists in '" << (*absolutePath) << "' but cannot be opened" << endl;
	  else {
    	cout << "cannot find config.ini file in '" << (*absolutePath) << "'" << endl;
			delete absolutePath;
		}
    return false;
  }
  
  //cout << "config path = " << _configFilePath << endl;  //delte !!!!!!!!!!!!!!!!!!!!
  
  
  ifstream myfile(_configFilePath.c_str());
  bool valid = false;
  if (myfile.is_open()) 
      valid = true;
  myfile.close();
  if(valid == false){
      string *absolutePath = GetAbsolutePath(_configFilePath);  //returns null on error, or prints in success
      if(absolutePath == NULL) {  //use relative path
	  cout << "config.ini exists in '" << _configFilePath << "' but cannot be opened" << endl;
      }
      else{
	  cout << "config.ini exists in '" << (*absolutePath) << "' but cannot be opened" << endl;
	  delete absolutePath;
      }
      
  }
  return valid;
}


string* SingletonConfigReader::GetAbsolutePath(string relativePath)
{
  
  char * relative = new char [relativePath.length()+1];
  std::strcpy (relative, relativePath.c_str());
  char actualpath [PATH_MAX + 1];
  char *ptr;
  ptr = realpath(relative, actualpath); 
  delete[] relative;
  std::string* abs = NULL;
  if(ptr != NULL){
    abs = new string(actualpath);  //calling function needs to delete abs in case it was allocateds
  }
  return abs;
  
//   std::string* mValue = NULL;
// 
// if (value) {
//     mValue = new std::string(value);
// }
}

//char *realpath(const char *restrict file_name, char *restrict resolved_name);


//         char buf[PATH_MAX + 1]; /* not sure about the "+ 1" */
//     char *res = realpath("this_source.c", buf);  ////  /home/ron2016linux2/Desktop/dir/noConfig/twoDocking.house
//     char *symlinkpath = "noConfig/twoDocking.house";
//     char *res = realpath("noConfig/twoDocking.house", buf);
//     char *res = realpath(symlinkpath, buf);
//     if (res) {
//         printf("This source is at %s.\n", buf);
//     } else {
//         perror("realpath");
//         exit(EXIT_FAILURE);
//     }
 
