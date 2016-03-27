#ifndef Point_h__
#define Point_h__

#include "stdafx.h"
#include "Direction.h"

namespace ns_robotic_cleaner
{

/*!
 * class Point
 * class Point is a class for point of two integers.
 */
class Point
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	unsigned int _x;
	unsigned int _y;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	Point(unsigned int x, unsigned int y);
	Point(const Point& other);
	virtual ~Point() {};
	void operator=(const Point &other );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	unsigned int GetX() const;
	void SetX(unsigned int x);
	unsigned int GetY() const;
	void SetY(unsigned int y);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool Move(Direction d); //Gets a direction and move the point in this direction (not getting to minus numbers)
};
	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

inline Point::Point(unsigned int x = 0, unsigned int y = 0) : _x(x), _y(y)
{
}

inline Point::Point(const Point& other) : _x(other._x), _y(other._y)
{
}

inline void Point::operator=(const Point &other )
{ 
	_x = other._x;
	_y = other._y;
}


inline unsigned int Point::GetX() const
{
	return _x;
}

inline void Point::SetX(unsigned int x)
{
	_x = x;
}

inline unsigned int Point::GetY() const
{
	return _y;
}

inline void Point::SetY(unsigned int y)
{
	_y = y;
}

//************************************
// Brief:		Gets a direction and move the point in this direction (not getting to minus numbers)
// Gets:	 	Direction d - where to move to 
// Returns:   	bool - true if the point was changed, false if remained the same (can't move)
// Access:    	public 
// Pre:			-
// Post:		-
//************************************
inline bool Point::Move(Direction d)
{
	switch (d)
	{
	case Direction::North :
		_y++;
		return true;
	case Direction::East :
		_x++;
		return true;
	case Direction::South :
		if (_y > 0){
			_y--;
			return true;
		}
		break;
	case Direction::West :
		if (_x > 0){
			_x--;
			return true;
		}
		break;
	}
	return false;  //we get here if direction is stay, or in case we are trying to move to negative
	//coordinates of the matrix.  
}

} // end of namespace ns_robotic_cleaner

#endif // Point_h__