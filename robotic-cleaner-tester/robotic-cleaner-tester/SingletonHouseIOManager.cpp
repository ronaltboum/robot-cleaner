//#include "stdafx.h"
#include "SingletonHouseIOManager.h"
#include <string>
#include <sstream>


namespace ns_robotic_cleaner_simulator
{
	SingletonHouseIOManager *SingletonHouseIOManager::s_instance = 0;

	SingletonHouseIOManager::SingletonHouseIOManager(void)
	{
	}
	
	
	SingletonHouseIOManager::~SingletonHouseIOManager(void)
	{
	}

	//************************************
	// Brief:		Read the houses in the folder and returns them as a vector of houses
	//				if no valid house found tries to look in default folder (the project folder)
	//************************************
	vector<House *> SingletonHouseIOManager::ReadHouses()
	{
		//throw std::logic_error("The method or operation is not implemented.");
		return vector<House *>();
	}

	House * SingletonHouseIOManager::ReadHouseFromFile(istream & in){
		string houseShortName, houseLongName, rowsString, colsString;
		string line;
		int rows, cols;
		getline (in,houseShortName);
		getline (in,houseLongName);
		getline (in,rowsString);
		if ( ! (istringstream(rowsString) >> rows) )
			return NULL;
		getline (in,colsString);
		if ( ! (istringstream(colsString) >> cols) )
			return NULL;
		House * h = new House(cols,rows,houseShortName,houseLongName);
		bool lessRowsThanExpected = false;
		int rowNumber;
		for(rowNumber = 0; rowNumber <rows ; ++rowNumber)
		{
			if(! getline (in,line) )
			{
				lessRowsThanExpected = true;
				break;
			}
			processRowFromFile(line, rowNumber, *h);
		}
		if(lessRowsThanExpected)
		{
			int rowsToFill = rows - rowNumber;
			fillLastRows(rowsToFill, *h);
		}
		if( getline (in,line) )
		{
			cout << "Warning: more rows than expected" << endl;
		}
		ValidateWallsAndCharacters(*h);
		if( ! h->isValid() )
		{
			delete h;
			h = NULL;
		}
		return h;
	}

	House * SingletonHouseIOManager::ReadHouseFromFile(string houseFileName)
	{
		ifstream myfile(houseFileName.c_str());

		if ( ! myfile.is_open()){
			cout << "Unable to open file" << endl;
			return NULL;
		}
		House * returnedHouse = ReadHouseFromFile(myfile);
		myfile.close();
		return returnedHouse;
	}

	//************************************
	// Brief:		Gets a row line from the text file and add it to the house rows
	// Gets:	 	string line - the line read from the text file
	// Gets:	 	int currentRowNumber - the row this line should be (according to the textfile)
	// Returns:   	void
	// Post:		floor(currentRowNumber,*)
	//************************************
	void SingletonHouseIOManager::processRowFromFile(string & rowFromFile, int currentRowNumber, House & h)
	{
		int cols = h.GetWidth();
		int len = rowFromFile.length();
		if(len < cols)
		{
			fillRow(rowFromFile, cols);
		}
		else if (len > cols)
		{
			rowFromFile = rowFromFile.substr(len - cols);
		}
		h.SetRow(rowFromFile,currentRowNumber);
	}

	//************************************
	// Brief:		Gets a reference to string and fill it be of length width
	// Pre:			line.length() < width
	// Post:		line.length() == width
	//************************************
	void SingletonHouseIOManager::fillRow(string & rowReadFromFile, int width)
	{
		if((rowReadFromFile.length() > 0) && (rowReadFromFile[rowReadFromFile.length() - 1] == d_cWallLetter))
		{
			rowReadFromFile[rowReadFromFile.length() - 1] = d_cSpaceLetter;
		}
		rowReadFromFile += string(width - rowReadFromFile.length(), d_cSpaceLetter);
		int newLength = rowReadFromFile.length();
		rowReadFromFile[0] = d_cWallLetter;
		rowReadFromFile[width - 1] = d_cWallLetter;
	}

	//************************************
	// Brief:		Gets a house by reference and fill it's linesToFill lines at the end
	// Gets:	 	int linesToFill
	// Gets:	 	House & h
	//************************************
	void SingletonHouseIOManager::fillLastRows(int rowsToFill, House & h)
	{
		int cols = h.GetWidth();
		int rows = h.GetHeight();
		string emptyWidthLine = wallString + string(cols - 2, d_cSpaceLetter) + wallString;
		string endLine = string(cols, d_cWallLetter);
		h.SetRow(endLine,rows -1); // last row
		for(int row = rows - rowsToFill; row < rows - 1 ; ++row)
		{
			h.SetRow(emptyWidthLine,row); 
		}
		cout << "Warning: less rows than expected, filled " << rowsToFill << " last rows"  << endl;
	}

	//************************************
	// Brief:		Gets a house
	//				fills all the surrounding with walls, 
	//				and replace all illegal chars with space
	//************************************
	void SingletonHouseIOManager::ValidateWallsAndCharacters(House & h)
	{
		unsigned int cols = h.GetWidth();
		unsigned int rows = h.GetHeight();
		if((cols == 0) || (rows == 0))
		{
			return ;
		}
		for(unsigned int row = 0; row < rows; ++row) 
		{
			for(unsigned int col = 0; col < cols; ++col) 
			{
				if((col == cols -1) || (col == 0) || (row == rows-1) || (row == 0))
				{
					h._floor(row,col) = d_cWallLetter;
					continue;
				}
				if(! h.IsValidTile( h._floor(row,col) ) )
					h._floor(row,col) = d_cSpaceLetter;
			}
		}
	}

}//end of namespace ns_robotic_cleaner_simulator

