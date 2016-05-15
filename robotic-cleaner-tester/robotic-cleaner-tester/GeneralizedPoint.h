#ifndef GeneralizedPoint_h__
#define GeneralizedPoint_h__

#include "Direction.h"

//using namespace std;

class GeneralizedPoint {   //a point that can also represent negative coordinates!  used in the implementations of algorithms
    
	int x, y;

public:
    GeneralizedPoint(int _x, int _y) : x(_x), y(_y) {}

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

//    friend std::ostream& operator<<(std::ostream& out, const GeneralizedPoint& p) {
//        return out << "(" << p.x << "," << p.y << ")";
//    }
};

#endif //GeneralizedPoint_h__
