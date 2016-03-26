#ifndef __HOUSE__H_
#define __HOUSE__H_

#include "stdafx.h"
#include "Point.h"
#include "Matrix.h"

namespace ns_robotic_cleaner
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
#define IsRegularTile(tile) ((('0' <= tile) &&(tile <= '9')))
#define IsDirty(tile) (!(('0' < tile) &&(tile <= '9')))
#define CharToNum(c) ((unsigned int)((c)-'0'))
#define d_sAllowedCharacters std::string("DW0123456789")

//~~~~~~~~~~~~~~~~~~~~` members ~~~~~~~~~~~~~~~~~~~~~~~~~`
	
private: 
	Matrix<char> floor;		//: the floor of the house
	
public:
	House(const unsigned int houseWidth, const unsigned int houseHeight);
	virtual ~House() {}
	
	//~~~~~~~~~~~~~~~~~`getters~~~~~~~~~~~~~~~~`
	unsigned int GetHeight() const;
	unsigned int GetWidth() const;		
	
	//~~~~~~~~~~~~~~~~~`functions~~~~~~~~~~~~~~~~
	bool IspositionValid(const Point & position) const; //: Gets a position and check if it's inside the house
	bool IsWall(const Point & position) const;			//: Gets a valid position and check if it's a wall
	unsigned int GetDirtLevel(const Point & position) const;		//: Gets a point and returns dirt level in it
	void SetDirtLevel(const Point & position, char charToSetTo); //: Gets a valid position and change the tile in it to be charToSetTo
	bool IsHouseClean() const; //: Check if all the tiles in the house are clean
};

inline unsigned int House::GetHeight() const
{
	return floor.getHeight();
}

inline unsigned int House::GetWidth() const
{
	return floor.getWidth();
}

inline bool House::IspositionValid(const Point& position) const
{
	return floor.IsInsideMatrix(position);
}

//************************************
// Brief:		Gets a valid position and check if it's a wall
// Pre:			IspositionValid(position)
// Post:		-
//************************************
inline bool House::IsWall(const Point& position) const
{
	return floor(position) == d_cWallLetter;
}

//************************************
// Brief:		Gets a valid position and change the tile in it to be charToSetTo
// Pre:			IspositionValid(position)
// Post:		-
//************************************
inline void House::SetDirtLevel(const Point & position, char charToSetTo)
{
	floor(position) = charToSetTo;
}

} // end of namespace ns_robotic_cleaner

#endif //__HOUSE__H_