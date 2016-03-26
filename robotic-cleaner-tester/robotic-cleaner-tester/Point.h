#ifndef Point_h__
#define Point_h__

#include "stdafx.h"

namespace ns_robotic_cleaner
{


class Point
{
private:
	unsigned int _x;
	unsigned int _y;
	
public:
	Point(unsigned int x, unsigned int y);
	virtual ~Point() {};
	Point(Point& other);
	unsigned int GetX() const;
	void SetX(unsigned int x);
	unsigned int GetY() const;
	void SetY(unsigned int y);
};
	
// ~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~

inline Point::Point(unsigned int x = 0, unsigned int y = 0) : _x(x), _y(y)
{
}

inline Point::Point(Point& other) : _x(other._x), _y(other._y)
{
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

} // end of namespace ns_robotic_cleaner

#endif // Point_h__