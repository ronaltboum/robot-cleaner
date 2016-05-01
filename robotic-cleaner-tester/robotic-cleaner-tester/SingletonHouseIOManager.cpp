//#include "stdafx.h"
#include "SingletonHouseIOManager.h"
#include <string>
#include <sstream>

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
	string houseShortName, houseLongName, rowsString, colsString, maxStepsString;
	string line;
	int rows, cols, maxSteps;
	
	std::string errorMessage;
	std::string errorOutput;
	std::stringstream sstm;
	
	getline (in,houseShortName);
	//getline (in,houseLongName);
	getline (in,maxStepsString);
	if ( (! (istringstream(maxStepsString) >> maxSteps)) || (maxSteps <= 0) ) { 
	 // cout << "i'm in bad translation of maxSteps" << endl;  // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	  //cout << "maxStepsString =  " << maxStepsString << endl; 
	  errorMessage = "line number 2 in house file shall be a positive number, found: ";
	  sstm << errorMessage << maxStepsString;
	  errorOutput = sstm.str(); 
	  House * h = new House(4,4,houseShortName,houseLongName, 88, errorOutput);  //4,4,88 are dummy values.
	  return h;  
	}

	getline (in,rowsString);
	if ( (! (istringstream(rowsString) >> rows)) || (rows <= 0) ) {
	  errorMessage = "line number 3 in house file shall be a positive number, found: ";
	  sstm << errorMessage << rowsString;
	  errorOutput = sstm.str(); 
	  House * h = new House(4,4,houseShortName,houseLongName, 88, errorOutput);  //4,4,88 are dummy values.
	  return h;  
		
	}
	getline (in,colsString);
	if ( (! (istringstream(colsString) >> cols)) || (cols <= 0) ){
	  errorMessage = "line number 4 in house file shall be a positive number, found: ";
	  sstm << errorMessage << colsString;
	  errorOutput = sstm.str(); 
	  House * h = new House(4,4,houseShortName,houseLongName, 88, errorOutput);  //4,4,88 are dummy values.
	  return h; 
	
	}
	
	House * h = new House(cols,rows,houseShortName,houseLongName, maxSteps, "");
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
	ValidateWallsAndCharacters(*h);  
	return h;  
}

House * SingletonHouseIOManager::ReadHouseFromFile(string houseFileName)
{
	
	string noSuffix = ExtractHouseName(houseFileName);
	ifstream myfile(houseFileName.c_str());
	if ( ! myfile.is_open()){ 
		return NULL;                     
	}
	House * returnedHouse = ReadHouseFromFile(myfile);
	returnedHouse -> SetHouseFileName(noSuffix);
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
	//int newLength = rowReadFromFile.length();
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
	//cout << "Warning: less rows than expected, filled " << rowsToFill << " last rows"  << endl;
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

vector<string> SingletonHouseIOManager::split(const string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

//returns the house name without the suffix.  for example:  for input dir/simple1.house, returns simple1
string SingletonHouseIOManager::ExtractHouseName(string houseFileName)
{
	char delim = '/';
	vector<string> nameSplit = split(houseFileName, delim);
	string houseName = nameSplit.back();
	delim = '.';
	vector<string> noSuffixVector = split(houseName, delim);
	int vecSize = noSuffixVector.size();
	string noSuffix;
	if(vecSize == 1) // .house
	  noSuffix = "";
	else
	  noSuffix = noSuffixVector[vecSize - 2];
	
	return noSuffix;
}

