#ifndef __HOUSE__H_
#define __HOUSE__H_

#include "Point.h"
#include "Matrix.h"
#include "SingletonHouseIOManager.h"
#include <sstream> 
#include <fstream>
#include <iostream>
#include <string>

class SingletonHouseIOManager;
/*
 * class House is representing a house which needs to be cleaned
 */	
class House 
{ 
friend class SingletonHouseIOManager;

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
#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
protected: 
	Matrix<char> _floor;			//: the floor of the house
	string _shortName;				//: the house short name
	string _longName;				//: the house long name

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
	friend istream& operator>>(istream& in, House& h); // getting from stream
private:
	void SetRow(const string & line, unsigned int rowNumber); // sets row rowNumber to be equal to line 	
protected:
	virtual bool IsValidTile(char tile) const; //: check that a tile is valid
	//virtual bool IsInMargins(Point & p) const; //: check if a point is in the margin of the house

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

inline bool House::IsValidTile(char tile) const
{
	return ( string("DW 123456789").find(tile) != string::npos); 
}

#endif //__HOUSE__H_
