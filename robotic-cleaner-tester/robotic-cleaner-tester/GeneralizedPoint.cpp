#include "GeneralizedPoint.h"

//using namespace std;

void GeneralizedPoint::move(int difx, int dify)
{
        x += difx;
        y += dify;
}

void GeneralizedPoint::move(Direction d) {     // reminder:  enum class Direction {East, West, South, North, Stay}
        switch(d) {
        case Direction(0) :
            return move(1, 0);
        case Direction(1) :
            return move(-1, 0);
        case Direction(2) :
            return move(0, 1);
        case Direction(3) :
            return move(0, -1);
        case Direction(4) :
            return move(0, 0);
        }
    }
