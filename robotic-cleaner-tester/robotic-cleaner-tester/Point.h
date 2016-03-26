#ifndef Point_h__
#define Point_h__

namespace (ns_robotic_cleaner) 
{


class Point
{
private:
	int _x;
	int _y;
	
public:
	Point();
	Point(int x, int y);
	~Point();
	Point(Point& other);
	int GetX() const;
	void SetX(int x);
	int GetY() const;
	void SetY(int y);
};
	
// ~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~
	
Point::Point() : _x(0.0), _y(0.0)
{
}
	
Point::Point(int x, int y) : _x(x), _y(y)
{
}
	
Point::Point(Point& other) : _x(other._x), _y(other._y)
{
}
	
Point::~Point()
{
}
	
int Point::GetX() const
{
	return _x;
}
	
void Point::SetX(int x)
{
	_x = x;
}
	
int Point::GetY() const
{
	return _y;
}
	
void Point::SetY(int y)
{
	_y = y;
}


} // end of namespace ns_robotic_cleaner

#endif // Point_h__