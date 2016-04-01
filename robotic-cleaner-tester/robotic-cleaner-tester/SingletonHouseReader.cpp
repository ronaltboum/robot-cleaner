#include "stdafx.h"
#include "SingletonHouseReader.h"
#include <string>


namespace ns_robotic_cleaner_simulator
{

	SingletonHouseReader::SingletonHouseReader(void)
	{
	}
	
	
	SingletonHouseReader::~SingletonHouseReader(void)
	{
	}

	//************************************
	// Brief:		Read the houses in the folder and returns them as a vector of houses
	//				if no valid house found tries to look in default folder (the project folder)
	//************************************
	vector<House *> SingletonHouseReader::ReadHouses()
	{
		throw std::logic_error("The method or operation is not implemented.");
		return vector<House *>();
	}

	House * SingletonHouseReader::ReadHouseFromFile(string houseFileName)
	{
		House * h = new House();
		string houseShortName, houseLongName, rowsString, colsString;
		string line;

		int rows, cols;
		ifstream myfile(houseFileName);
		
		if ( ! myfile.is_open()){
			cout << "Unable to open file" << endl;
			return false;
		}
		
		getline (myfile,houseShortName);
		getline (myfile,houseLongName);
		getline (myfile,rowsString);
		if ( ! (istringstream(rowsString) >> rows) )
			return NULL;
		getline (myfile,colsString);
		if ( ! (istringstream(colsString) >> cols) )
			return NULL;
		//floor = Matrix<char>(rows,cols);
		//int rowNumber = 1;
		//while ( getline (myfile,line) )
		//{
		//	processLine(line, rowNumber);
		//}
		//myfile.close();
		//return true;
		return NULL;
	}

	//************************************
	// Brief:		Gets a row line from the text file and add it to the house rows
	// Gets:	 	string line - the line read from the text file
	// Gets:	 	int currentRowNumber - the row this line should be (according to the textfile)
	// Returns:   	void
	// Post:		floor(currentRowNumber,*)
	//************************************
	void House::processLine(string line, int currentRowNumber)
	{
		int cols = GetWidth();
		int rows = GetHeight();
		throw std::logic_error("The method or operation is not implemented.");
	}

}//end of namespace ns_robotic_cleaner_simulator

