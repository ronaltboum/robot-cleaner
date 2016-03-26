#ifndef __HOUSE__H_
#define __HOUSE__H_

#include <stdio.h>
#include "Point.h"
#include "Matrix.h"

namespace (ns_robotic_cleaner) 
{

	
class House 
{ 

//~~~~~~~~~~~~~~~~~~~~` defines ~~~~~~~~~~~~~~~~~~~~~~~~~`
#define d_cWallLetter 'W'
#define d_cDockingLetter 'D'
#define IsRegularTile(tile) ((('0' <= tile) &&(tile <= '9')))
#define IsDirty(tile) (!(('0' < tile) &&(tile <= '9')))
#define CharToNum(c) ((int)((c)-'0'))
#define d_sAllowedCharacters std::string("DW0123456789")

//~~~~~~~~~~~~~~~~~~~~` members ~~~~~~~~~~~~~~~~~~~~~~~~~`
	
private: 
	Matrix<char> houseFloor;		//: the floor of the house
	
public:
	House(const int houseWidth, const int houseHeight);
	virtual ~House() {}
	
	//~~~~~~~~~~~~~~~~~`getters~~~~~~~~~~~~~~~~`
	int GetHeight() const;
	int GetWidth() const;		
	
	//~~~~~~~~~~~~~~~~~`functions~~~~~~~~~~~~~~~~
	bool IsLocationValid(const Point & location) const; //: Gets a location and check if it's inside the house
	bool IsWall(const Point & location) const;			//: Gets a valid location and check if it's a wall
	int GetDirtLevel(const Point & location) const;		//: Gets a point and returns dirt level in it
	bool IsHouseClean() const; //: Check if all the tiles in the house are clean
};

inline int House::GetHeight() const
{
	return houseFloor.getHeight();
}

inline int House::GetWidth() const
{
	return houseFloor.getWidth();
}

inline bool House::IsLocationValid(const Point& location) const
{
	return houseFloor.IsInsideMatrix(location);
}

//************************************
// Brief:		Gets a valid location and check if it's a wall
// Gets:	 	int w, int h
// Returns:   	bool
// Access:    	public 
// Pre:			IsLocationValid(w,h)
// Post:		-
//************************************
inline bool House::IsWall(const Point& location) const
{
	return houseFloor(location) == d_cWallLetter;
}

} // end of namespace ns_robotic_cleaner

#endif //__HOUSE__H_