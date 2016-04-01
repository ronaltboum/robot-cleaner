#include "stdafx.h"
#include "House.h"
#include <string>

namespace ns_robotic_cleaner_simulator
{
	House::House()
	{
		initiallize();
		_shortName = string("House1");
		char house[19][81] = {
		//             1         2         3         4         5         6         7        
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"W123WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
			"10 79   D              1234321                                                 W", // 1 =  
			"W  98      WWWWWWW     1234321                     W                       1   W", // 2 =  
			"W              W                                   W   555                 2   W", // 3 =  
			"W              W                                   W   555                 3   W", // 4 =  282
			"W              W           WWWWWWWWWWWWWWWWWWWWWWWWW                       4   W", // 5 =  
			"W              W                                                           5   W", // 6 =  
			"W              W                                                           6   W", // 7 =  
			"W                          WWWWWWWWWWWWWWWWWWWWWW  WWWWWWW                 7   W", // 8 =  24?
			"W         1         2         3         4         5W 999 W  6         7        W", // 9 =  
			"W              W           444                     W 999 W                 9   W", // 10 = 
			"W              W           444                     W 999 W                 8   W", // 11 =
			"W              W                                   W     W                 7   W", // 12 = 
			"W              W                                   WW   WW                 6   W", // 13 = 
			"W              W                                    W   W                  5   W", // 14 = 
			"W              W                                                           4   W", // 15 = 
			"W              W                                                           3   W", // 16 = 
			"W              W                                                               W", // 17
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" // 18
		};
		unsigned int rows = ARRAY_SIZE(house); //19
		unsigned int cols = ARRAY_SIZE(house[0]); //81
		_floor = Matrix<char>(rows,cols);
		for(unsigned int row = 0; row < rows; ++row) {
			for(unsigned int col = 0; col < cols; ++col) {
				char c = house[row][col];
				_floor(row,col) = house[row][col];
			}
		}
		ValidateWallsAndCharacters();
		cout << *this;
	}

	House::House(unsigned int houseWidth, unsigned int houseHeight)
	{
		initiallize();
		_floor = Matrix<char>(houseWidth, houseHeight);
		
	}
	
	House::House(unsigned int houseWidth, unsigned int houseHeight, string shortName, string longName)
	{
		initiallize();
		_floor = Matrix<char>(houseWidth, houseHeight);
		_shortName = shortName;
		_longName = longName;
	}


	//************************************
	// Brief:	used to initiallize the house, called first in each c'tor
	//************************************
	void House::initiallize()
	{
		_floor = Matrix<char>(0, 0);
		_shortName = "";
		_longName = "";
	}

	//************************************
	// Brief:		Fills all the surrounding with walls, and replace all illegal chars with space
	//************************************
	void House::ValidateWallsAndCharacters()
	{
		unsigned int lastColIndex = GetWidth() - 2; //not including \0 col in the end
		unsigned int lastRowIndex = GetHeight() - 1;
		if((lastColIndex < 0) || (lastRowIndex < 0))
		{
			return ;
		}
		for(unsigned int row = 0; row <= lastRowIndex; ++row) 
		{
			for(unsigned int col = 0; col <= lastColIndex; ++col) 
			{
				if((col == lastColIndex) || (col == 0) || (row == lastRowIndex) || (row == 0))
				{
					_floor(row,col) = d_cWallLetter;
					continue;
				}
				if(! IsValidTile( _floor(row,col) ) )
					_floor(row,col) = d_cSpaceLetter;
			}
		}
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

	//************************************
	// Brief:		Print a single row
	//************************************
	void House::PrintRow(unsigned int row) const
	{
		assert(row < GetHeight() && row >= 0);
		unsigned int cols = GetWidth();
		for(unsigned int col = 0; col < cols; ++col) {
			cout << _floor(row,col);
		}
	}

	void House::Print() const
	{
		unsigned int rows = GetHeight();
		for(unsigned int row = 0; row < rows; ++row) {
			PrintRow(row);
			cout << endl;
		}
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

	bool House::isValid() const /* check that the house is valid */ 
	{
		bool dockingStationExists = false;
		unsigned int lastColIndex = GetWidth() - 2; //not including \0 col in the end
		unsigned int lastRowIndex = GetHeight() - 1;
		if((lastColIndex < 2) || (lastRowIndex < 2)) //at least 3x3 in order to be surrounded by walls
		{
			return false;
		}
		for(unsigned int row = 0; row <= lastRowIndex; ++row) 
		{
			for(unsigned int col = 0; col <= lastColIndex; ++col) 
			{
				if( ! IsValidTile( _floor(row,col) ) )
					return false;
				if((col == lastColIndex) || (col == 0) || (row == lastRowIndex) || (row == 0))
				{
					if( ! IsWallTile(_floor(row,col)) )
						return false;
				}
				else if( IsDockingTile(_floor(row,col)) )
				{
					if(dockingStationExists)
						return false; //two docking stations
					dockingStationExists = true;
				}
			}
		}
		return dockingStationExists;
	}

	std::ostream& operator<<(std::ostream& out, const House& houseToPrint)
	{
		unsigned int lastColIndex = houseToPrint.GetWidth() - 2; //not including \0 col in the end
		unsigned int lastRowIndex = houseToPrint.GetHeight() - 1;
		for(unsigned int row = 0; row <= lastRowIndex; ++row) 
		{
			for(unsigned int col = 0; col <= lastColIndex; ++col) 
			{   
				out << houseToPrint._floor(row,col);
			}   
			cout << endl; 
		}
		return out;
	}

} // end of namespace ns_robotic_cleaner_simulator



