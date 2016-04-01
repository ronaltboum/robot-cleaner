#include "stdafx.h"
#include "SingletonConfigReader.h"
#include <iosfwd>
#include <sstream> 
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

namespace ns_robotic_cleaner_simulator
{
SingletonConfigReader *SingletonConfigReader::s_instance = 0;
const SingletonConfigReader::StringToIntMap SingletonConfigReader::defaultValues = initDefaultValues();

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

bool SingletonConfigReader::processLine(const string& line, map<string, int> & configMap)
{
	vector<string> tokens = split(line, '=');
	if (tokens.size() != 2)
	{
		return false;
	}
	int parameterValue = 0;
	if ( ! (istringstream(tokens[1]) >> parameterValue) )
		return false;
	configMap[trim(tokens[0])] = parameterValue;
	return true;
}

void SingletonConfigReader::CompleteMissingConfigs(map<string, int> & configs)
{
	for(auto configNameValuePair : defaultValues)
	{
		if ( configs.find(configNameValuePair.first) == configs.end() ) {
			cout << "value of " << configNameValuePair.first << " not set, using default";
			configs.insert(configNameValuePair);
		}
	}
	
}

map<string, int> SingletonConfigReader::ReadConfigFromFile()
{
	string line;
	ifstream myfile(_configFilePath);
	int lineNumber = 1;
	map<string, int> configs = map<string,int>();
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
			if( ! processLine(line, configs))
			{
				cout << "Warning : Problem parsing line number: " << lineNumber << " in the config file";
			}
		}
		myfile.close();
	}
	else{
		cout << "Unable to open file" << endl;
		if(_configFilePath != configFileName){
			cout << "trying to find in default folder" << endl;
			_configFilePath = configFileName;
			return ReadConfigFromFile();
		}
		else{
			cout << "file doesn't exist in default folder" << endl;
		}
	}
	CompleteMissingConfigs(configs);
	return configs;
}

} //end of namespace ns_robotic_cleaner_simulator
