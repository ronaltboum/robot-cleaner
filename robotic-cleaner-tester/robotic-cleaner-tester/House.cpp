//#include "stdafx.h"
#include "House.h"
#include <string>

namespace ns_robotic_cleaner_simulator
{
	House::House()
	{
		initiallize();
	}

	House::House(unsigned int houseWidth, unsigned int houseHeight)
	{
		initiallize();
		_floor = Matrix<char>(houseHeight,houseWidth);
		
	}
	
	House::House(unsigned int houseWidth, unsigned int houseHeight, string shortName, string longName)
	{
		initiallize();
		_floor = Matrix<char>(houseHeight,houseWidth);
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
	// Returns:   	bool
	//************************************
	bool House::isValid() const
	{
		bool dockingStationExists = false;
		unsigned int cols = GetWidth(); //not including \0 col in the end
		unsigned int rows = GetHeight();
		if((cols < 3) || (rows < 3)) //at least 3x3 in order to be surrounded by walls
		{
			return false;
		}
		for(unsigned int row = 0; row < rows; ++row) 
		{
			for(unsigned int col = 0; col < cols; ++col) 
			{
				if( ! IsValidTile( _floor(row,col) ) )
					return false;
				if((col == cols-1) || (col == 0) || (row == rows-1) || (row == 0))
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


	//************************************
	// Brief:		Sets the house #rowNumber row to be rowAsString
	// Pre:			rowAsString.length() == GetWidth();
	//************************************
	void House::SetRow(const string & rowAsString, unsigned int rowNumber)
	{
		int cols = GetWidth();
		if(rowAsString.length() != cols)
			return;
		for(int col = 0; col < cols; ++col)
		{
			_floor(rowNumber,col) = rowAsString[col];
		}
	}

	std::ostream& operator<<(std::ostream& out, const House& houseToPrint)
	{
		unsigned int cols = houseToPrint.GetWidth();
		unsigned int rows = houseToPrint.GetHeight();
		out << houseToPrint._shortName << endl;
		out << houseToPrint._longName << endl;
		out << rows << endl;
		out << cols << endl;
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
		House * houseRead = new House();
		House * a = SingletonHouseIOManager::instance()->ReadHouseFromFile("house1.house");
		if(houseRead == NULL)
			in.setstate(std::ios::failbit);
		/*else
			this = (*houseRead);*/
		return in;
	}

} // end of namespace ns_robotic_cleaner_simulator



