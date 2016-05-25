#ifndef GeneralizedPoint_h__
#define GeneralizedPoint_h__

#include "Direction.h"
#include "DirectionExt.h"
#include <iostream>
#include <vector>

using namespace std;

class GeneralizedPoint {   //a point that can also represent negative coordinates!  used in the implementations of algorithms
    
	int x, y;

public:
    GeneralizedPoint(int _x = 0, int _y =0) : x(_x), y(_y) {}

    GeneralizedPoint distance(const GeneralizedPoint& other)const {
        return GeneralizedPoint(x - other.x, y - other.y);
    }

    int getX()const {return x;}
    int getY()const {return y;}

    //GeneralizedPoint& adjustToScreen(size_t width, size_t height);
//    GeneralizedPoint& adjustToScreen(size_t width, size_t height) {
//        x = (x + ((x<0? -x: x) / width + 1) * width) % width;
//        y = (y + ((y<0? -y: y) / height + 1) * height) % height;
//        return *this;
//    }

    void move(int difx, int dify);

    void move(Direction d);     // reminder:  enum class Direction {East, West, South, North, Stay}
        
    // required by map<GeneralizedPoint, T>
    bool operator<(const GeneralizedPoint& other)const {
        return (x < other.x || (x == other.x && y < other.y)); 
    }

    bool operator==(const GeneralizedPoint& rhs) const
    {
        return ! ( (*this) < rhs || rhs < (*this) );
    }

    bool operator!=(const GeneralizedPoint& rhs) const
    {
        return !((*this) == rhs);
    }

    Direction GetDirection(const GeneralizedPoint & other) const;

    string ToString() const{
        return string("(") + std::to_string(x) + string(",") + std::to_string(y) + string(")");
    }  

    static string ToStringPath(const vector<GeneralizedPoint> & path);

   friend ostream& operator<<(ostream& out, const GeneralizedPoint& p){
       return out << p.ToString();
   }

 
};

#endif //GeneralizedPoint_h__
