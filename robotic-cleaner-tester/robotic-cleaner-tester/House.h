#ifndef __HOUSE__H_
#define __HOUSE__H_

#include "Point.h"
#include "Matrix.h"
#include <iostream>

namespace ns_robotic_cleaner_simulator
{

	
class House 
{ 
 /*!
 * \class House
 * class House is representing a house in the memory
 */

//~~~~~~~~~~~~~~~~~~~~` defines ~~~~~~~~~~~~~~~~~~~~~~~~~`
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

//~~~~~~~~~~~~~~~~~~~~` members ~~~~~~~~~~~~~~~~~~~~~~~~~`
	
private: 
	Matrix<char> floor;		//: the floor of the house
	string * shortName;

public:
	House(const unsigned int houseWidth, const unsigned int houseHeight);
	House(); // :Default C'tor using hardcoded house
	virtual ~House() {delete shortName;}
	
	//~~~~~~~~~~~~~~~~~`getters~~~~~~~~~~~~~~~~`
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;		
	string GetShortName() const { return * shortName; }
	
	//~~~~~~~~~~~~~~~~~`functions~~~~~~~~~~~~~~~~
	bool IsPositionValid(const Point & position) const; //: Gets a position and check if it's inside the house
	bool IsWall(const Point & position) const;			//: Gets a valid position and check if it's a wall
	bool IsDockingStation(const Point & position) const;			//: Gets a valid position and check if it's a docking station
	bool IsDirty(const Point & location) const;
	bool IsClean() const; //: Check if all the tiles in the house are clean
	unsigned int GetDirtLevel(const Point & position) const;		//: Gets a point and returns dirt level in it
	Point * GetDockingStation() const;
	void SetTile(const Point & position, char charToSetTo); //: Gets a valid position and change the tile in it to be charToSetTo
	int Clean(const Point & position); //: Gets a valid position and if it's dirty set its dirt level to be current level -1
	void Print() const;
	void Print(const Point & currentPosition) const; //prints the house while the current position marked with '*'
	unsigned int SumOfDirtInTheHouse() const; //: Sums how much dirt there is in the house
	bool isValid() const; // check that the house is valid
	
private:
	void PrintRow(unsigned int row) const;
	void ValidateWallsAndCharacters();
};

inline unsigned int House::GetHeight() const
{
	return floor.getHeight();
}

inline unsigned int House::GetWidth() const
{
	return floor.getWidth();
}

inline bool House::IsPositionValid(const Point& position) const
{
	return floor.IsInsideMatrix(position);
}

//************************************
// Brief:		Gets a valid position and check if it's a wall
// Pre:			IsPositionValid(position)
// Post:		-
//************************************
inline bool House::IsWall(const Point& position) const
{
	return IsWallTile(floor(position));
}

//************************************
// Brief:		Gets a valid position and check if it's a docking station
// Access:    	public 
// Pre:			IsPositionValid(position)
//************************************
inline bool House::IsDockingStation(const Point & position) const
{
	return IsDockingTile(floor(position));
}

//************************************
// Brief:		Gets a valid position and change the tile in it to be charToSetTo
// Pre:			IsPositionValid(position)
// Post:		-
//************************************
inline void House::SetTile(const Point & position, char charToSetTo)
{
	floor(position) = charToSetTo;
}

//************************************
// Brief:		Gets a valid position and if it's dirty set its dirt level to be current level -1
// Gets:	 	const Point & position
// Returns:   	1 if cleaned something, zero otherwise.
// Access:    	public 
// Pre:			IsPositionValid(position)
// Post:		-
//************************************
inline int House::Clean(const Point & position) 
{
	int amountCleaned = 0;
	if(IsDirtTile(floor(position)))
	{
		floor(position)--;
		if(floor(position) == '0')
			floor(position) = ' ' ;
		amountCleaned = 1;
	}
	return amountCleaned;
}

} // end of namespace ns_robotic_cleaner_simulator

#endif //__HOUSE__H_