#ifndef __HOUSE__H_
#define __HOUSE__H_

#include "Point.h"
#include "Matrix.h"
#include <sstream> 
#include <fstream>
#include <iostream>
#include <string>

namespace ns_robotic_cleaner_simulator
{

/*
 * class House is representing a house which needs to be cleaned
 */	
class House 
{ 
friend class HouseFileReader;

//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
#define d_cWallLetter 'W'
#define d_cDockingLetter 'D'
#define	d_cSpaceLetter ' '
#define IsDirtTile(tile) (('1' <= tile) && (tile <= '9'))
#define IsSpaceTile(tile) ((tile == ' '))
#define IsWallTile(tile) ((tile == d_cWallLetter))
#define IsDockingTile(tile) ((tile == d_cDockingLetter))
#define IsRegularTile(tile) (IsDirtTile(tile) || IsSpaceTile(tile))
#define CharToNum(c) ((unsigned int)((c)-'0'))
#define d_sAllowedCharacters string("DW 123456789")
#define IsValidTile(tile) ((d_sAllowedCharacters.find(tile) != string::npos))
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private: 
	Matrix<char> _floor;		//: the floor of the house
	string _shortName;		//: the house short name
	string _longName;		//: the house long name

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	House(); // :Default C'tor using hardcoded house
	House(unsigned int houseWidth, unsigned int houseHeight);
	House(unsigned int houseWidth, unsigned int houseHeight, string shortName, string longName);
	virtual ~House() {}
	void initiallize(); // used to initiallize house in each c'tor
	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;		
	string GetShortName() const { return _shortName; }
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool IsPositionValid(const Point & position) const; //: Gets a position and check if it's inside the house
	bool IsWall(const Point & position) const;			//: Gets a valid position and check if it's a wall
	bool IsDockingStation(const Point & position) const;			//: Gets a valid position and check if it's a docking station
	bool IsDirty(const Point & location) const;			//: check if a point is dirty
	bool IsHouseClean() const; //: Check if all the tiles in the house are clean
	unsigned int GetDirtLevel(const Point & position) const;		//: Gets a point and returns dirt level in it
	Point * GetDockingStation() const; //: returns a new point of where the docking station is, null if doesn't exist
	void SetTile(const Point & position, char charToSetTo); //: Gets a valid position and change the tile in it to be charToSetTo
	int Clean(const Point & position); //: Gets a valid position and if it's dirty set its dirt level to be current level -1
	void Print() const;
	void Print(const Point & currentPosition) const; //: prints the house while the current position marked with '*'
	unsigned int SumOfDirtInTheHouse() const; //: Sums how much dirt there is in the house
	bool isValid() const; //: check that the house is valid
	friend std::ostream& operator << ( std::ostream& out, const House& houseToPrint ); //printing to stream
	bool ReadConfigFromFile(string houseFileName); // TODO: split folder and move to private
private:
	void PrintRow(unsigned int row) const; //: print one row in the house
	void ValidateWallsAndCharacters(); //: check that all characters are valid, and change them if not. puts walls in the sides.
	void processLine(string line, int currentRowNumber);
	
		
	//friend istream& operator>>(istream& in, House& vec) 
	//{
	//	for (int row = 0; row < boardSizeRow; row++)
	//	{
	//		for (int col = 0; col < boardSizeCol; col++)
	//		{   
	//			inputFile >> gameBoard[row][col];
	//		}   
	//	}
	//	// read obj from stream

	//	if( /* no valid object of T found in stream */ )
	//		in.setstate(std::ios::failbit);

	//	return in;
	//}


};


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

inline unsigned int House::GetHeight() const
{
	return _floor.getHeight();
}

inline unsigned int House::GetWidth() const
{
	return _floor.getWidth();
}

//************************************
// Brief:		Gets a position and checks if it's inside the house
// Returns:   	bool
//************************************
inline bool House::IsPositionValid(const Point& position) const
{
	return _floor.IsInsideMatrix(position);
}

//************************************
// Brief:		Gets a valid position and check if it's a wall
// Pre:			IsPositionValid(position)
// Post:		-
//************************************
inline bool House::IsWall(const Point& position) const
{
	return IsWallTile(_floor(position));
}

//************************************
// Brief:		Gets a valid position and check if it's a docking station
// Pre:			IsPositionValid(position)
//************************************
inline bool House::IsDockingStation(const Point & position) const
{
	return IsDockingTile(_floor(position));
}

//************************************
// Brief:		Gets a valid position and change the tile in it to be charToSetTo
// Pre:			IsPositionValid(position)
// Post:		-
//************************************
inline void House::SetTile(const Point & position, char charToSetTo)
{
	_floor(position) = charToSetTo;
}

//************************************
// Brief:		Gets a valid position and if it's dirty set its dirt level to be current level -1
// Returns:   	1 if cleaned something, zero otherwise.
// Pre:			IsPositionValid(position)
//************************************
inline int House::Clean(const Point & position) 
{
	int amountCleaned = 0;
	if(IsDirtTile(_floor(position)))
	{
		_floor(position)--;
		if(_floor(position) == '0')
			_floor(position) = ' ' ;
		amountCleaned = 1;
	}
	return amountCleaned;
}

} // end of namespace ns_robotic_cleaner_simulator

#endif //__HOUSE__H_