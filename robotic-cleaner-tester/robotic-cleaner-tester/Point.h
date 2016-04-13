#ifndef Point_h__
#define Point_h__

#include "Direction.h"

namespace ns_robotic_cleaner_simulator
{

/*
 * class Point is a class for point of two integers.
 */
class Point
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	unsigned int _row;
	unsigned int _col;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	Point(unsigned int row, unsigned int col);
	Point(const Point& other);
	virtual ~Point() {};
	void operator=(const Point &other );

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	unsigned int GetRow() const;
	void SetRow(unsigned int row);
	unsigned int GetCol() const;
	void SetCol(unsigned int col);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool Move(Direction d); //Gets a direction and move the point in this direction (not getting to minus numbers)
	bool operator ==(const Point& other) const;
	bool operator !=(const Point& other) const;

};

	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

inline bool Point::operator ==(const Point& other) const
{
	return (_col == other._col && _row == other._row);
}

inline bool Point::operator !=(const Point& other) const
{
	return !(*this == other);
}

inline void Point::operator=(const Point &other )
{
	_row = other._row;
	_col = other._col;
}

inline Point::Point(unsigned int row = 0, unsigned int col = 0) : _row(row), _col(col)
{
}

inline Point::Point(const Point& other) : _row(other._row), _col(other._col)
{
}


inline unsigned int Point::GetRow() const
{
	return _row;
}

inline void Point::SetRow(unsigned int row)
{
	_row = row;
}

inline unsigned int Point::GetCol() const
{
	return _col;
}

inline void Point::SetCol(unsigned int col)
{
	_col = col;
}

//************************************
// Brief:		Gets a direction and move the point in this direction (not getting to minus numbers)
// Gets:	 	Direction d - where to move to 
// Returns:   	bool - true if the point was changed, false if remained the same (can't move)
//************************************
inline bool Point::Move(Direction d)
{
	switch (d)
	{
	case Direction(3) :
		if (_row > 0){
			_row--;
			return true;
		}
		break;
	case Direction(2) :
		_row++;
		return true;
	case Direction(0) :
		_col++;
		return true;
	case Direction(1) :
		if (_col > 0){
			_col--;
			return true;
		}
		break;
	case Direction(4):
	default:
		return false;
	}
}


} // end of namespace ns_robotic_cleaner_simulator

#endif // Point_h__