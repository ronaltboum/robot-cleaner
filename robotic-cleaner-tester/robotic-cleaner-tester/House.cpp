//#include "stdafx.h"
#include "House.h"
#include <string>

House::House()
{
	initiallize();
}

House::House( int houseWidth,  int houseHeight)
{
	initiallize();
	_floor = Matrix<char>(houseHeight,houseWidth);
	
}

House::House(int houseWidth,  int houseHeight, string shortName, string longName, int maxSteps, string errorMessage)
{
	initiallize();
	_floor = Matrix<char>(houseHeight,houseWidth);
	_shortName = shortName;
	_longName = longName;
	_MaxSteps = maxSteps;
	_errorMessage = errorMessage;
}

//Matrix (vector<T> inner, unsigned int rows, unsigned int cols);
House::House(int houseWidth,  int houseHeight, vector<char> inner, int maxSteps, string errorMessage, string houseFileName)
{
  _floor = Matrix<char> (inner, houseHeight, houseWidth);
  _MaxSteps = maxSteps;
  _errorMessage = errorMessage;
  _houseFileName = houseFileName;
}

//************************************
// Brief:	used to initiallize the house, called first in each c'tor
//************************************
void House::initiallize()
{
	_floor = Matrix<char>(0, 0);
	_shortName = "";
	_longName = "";
	_MaxSteps = 0;
	_errorMessage = "";
	_houseFileName = "";
	
	//_isWinnerAlgorithmExist = false;
}


//************************************
// Brief:		Returns a pointer to new point containing the Docking Station
// Returns:   	Point pointer if found, null if not
// Post:		Point allocated
//************************************
Point * House::GetDockingStation() const
{
	unsigned int lastColIndex = GetWidth() - 2; //not including \0 col in the end
	unsigned int lastRowIndex = GetHeight() - 1;
	for(unsigned int row = 0; row <= lastRowIndex; ++row) 
	{
		for(unsigned int col = 0; col <= lastColIndex; ++col) 
		{
			if(IsDockingTile(_floor(row,col) ))
			{
				return new Point(row,col);
			}
		}
	}
	return NULL;
}

//************************************
// Brief:		Gets a location and returns the dirt level in it
// Returns:   	int 0-9
// Pre:			IsPositionValid(position)
//************************************
unsigned int House::GetDirtLevel(const Point & location) const
{
	char tileContent = _floor(location);
	if(IsDirtTile(tileContent))
		return CharToNum(tileContent);
	return 0;
}

//************************************
// Brief:		Gets a location and checks if it's dirty
// Returns:   	int 0-9
// Pre:			IsPositionValid(position)
//************************************
bool House::IsDirty(const Point & location) const
{
	return (GetDirtLevel(location) > 0);
}

//************************************
// Brief:		Check if all the tiles in the house are clean
//************************************
bool House::IsHouseClean() const
{
	unsigned int lastColIndex = GetWidth() - 2; //not including \0 col in the end
	unsigned int lastRowIndex = GetHeight() - 1;
	for(unsigned int row = 0; row <= lastRowIndex; ++row) 
	{
		for(unsigned int col = 0; col <= lastColIndex; ++col) 
		{
			if(IsDirtTile(_floor(row,col) ))
			{
				return false;
			}
		}
	}
	return true;
}


void House::Print() const
{
	cout << (*this);
}

void House::Print(const Point & currentPosition) const
{
	unsigned int rows = GetHeight();
	unsigned int cols = GetWidth();
	for(unsigned int row = 0; row < rows; ++row) {
		for(unsigned int col = 0; col < cols; ++col) {
			if(currentPosition == Point(row,col))
				cout << '*';
			else
				cout << _floor(row,col);
		}
		cout << endl;
	}

}

unsigned int House::SumOfDirtInTheHouse() const
{
	unsigned int sum = 0;
	unsigned int lastColIndex = GetWidth() - 2; //not including \0 col in the end
	unsigned int lastRowIndex = GetHeight() - 1;
	char tileContent;
	for(unsigned int row = 0; row <= lastRowIndex; ++row) 
	{
		for(unsigned int col = 0; col <= lastColIndex; ++col) 
		{
			if(IsDirtTile(_floor(row,col) ))
			{
				tileContent = _floor(row,col);
				sum += (CharToNum(tileContent));
			}
		}
	}
	return sum;
}

//************************************
// Brief:		Checks that the house is valid:
//					1. Walls on the sides
//					2. one docking station
//					3. no illegal characters
// Returns:   	"valid" or error message 
// pre:         the function is called AFTER we fixed the house in function SingletonHouseIOManager::ReadHouseFromFile(istream & in)
//************************************
std::string House::isValid() const
{
	std::string errorMessage;
	std::string errorOutput;
	std::stringstream sstm;
	bool dockingStationExists = false;
	
	std::string error_message = GetErrorMessage();
	if(error_message.compare("") != 0 ){   //error_message != ""
	  return error_message;
	}


	unsigned int rows = GetHeight();
	unsigned int cols = GetWidth(); //not including \0 col in the end
	for(unsigned int row = 0; row < rows; ++row) 
	{
		for(unsigned int col = 0; col < cols; ++col) 
		{

			//the check of illegal char and IsWallTile is useless, since we call this
			//function after we fixed the house when we read it from the file

			//if( ! IsValidTile( _floor(row,col) ) )
			//	return "illegal char in house"; 
			if( !( (col == cols-1) || (col == 0) || (row == rows-1) || (row == 0) ) )
			{ //in case we are not at the edges:
				//if( ! IsWallTile(_floor(row,col)) ) //we check this after we fix the house!
				//	return false;
				if( IsDockingTile(_floor(row,col)) )
				{
					if(dockingStationExists)
						return "too many docking stations (more than one D in house)"; //two docking stations
					dockingStationExists = true;
				}
			}
			 
		}
	}
	//return dockingStationExists;
	//check if there a docking station exists:
	if(dockingStationExists == false)
		return "missing docking station (no D in house)";
	return "valid";
}


//************************************
// Brief:		Sets the house #rowNumber row to be rowAsString
// Pre:			rowAsString.length() == GetWidth();
//************************************
void House::SetRow(const string & rowAsString, unsigned int rowNumber)
{
	unsigned int cols = GetWidth();
	if(rowAsString.length() != cols)
		return;
	for(unsigned int col = 0; col < cols; ++col)
	{
		_floor(rowNumber,col) = rowAsString[col];
	}
}

std::ostream& operator<<(std::ostream& out, const House& houseToPrint)
{
	unsigned int cols = houseToPrint.GetWidth();
	unsigned int rows = houseToPrint.GetHeight();
	out << houseToPrint._shortName << ":" << houseToPrint._longName << " " << cols << "x" << rows << endl;
	for(unsigned int row = 0; row < rows; ++row) 
	{
		for(unsigned int col = 0; col < cols; ++col) 
		{   
			out << houseToPrint._floor(row,col);
		}   
		out << endl; 
	}
	return out;
}

std::istream& operator>>(istream& in, House& h) 
{
	House * houseRead = SingletonHouseIOManager::instance()->ReadHouseFromFile(in);
	if(houseRead == NULL)
		in.setstate(std::ios::failbit);
	else
		h = House(*houseRead);
	delete houseRead ;
	return in;
}


void House::Montage(const Point & currentPosition, const string & imageDir, int stepNumber) const
{
	unsigned int rows = GetHeight();
	unsigned int cols = GetWidth();
	vector<string> houseImages;
	for(unsigned int row = 0; row < rows; ++row) {
		for(unsigned int col = 0; col < cols; ++col) {
			if(currentPosition == Point(row,col))
				houseImages.push_back("avatars/R");
			else if (_floor(row,col) == ' ')
				houseImages.push_back("avatars/0");
			else
				houseImages.push_back(string("avatars/") + _floor(row,col) ) ;
		}
	}
	string indexStr = to_string(stepNumber);
	string composedImage = "simulations/" + imageDir + "/image" + string(5-indexStr.length(), '0') + indexStr + ".jpg";
	//	cout << "composing" << composedImage << endl;
	Montage::compose(houseImages, cols, rows, composedImage);
}
