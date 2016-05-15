#ifndef DirectionExt_h__
#define DirectionExt_h__

#include "Direction.h"
#include <ostream>

using namespace std;

// prefix++ for Direction, skipping Stay
inline Direction operator++(Direction& d) {
    d = (Direction)(((int)d + 1)%4);
    return d;
}

// added for debug purposes
inline ostream& operator<<(ostream& out, Direction d) {
    static const char* desc[] = {"East", "West", "South", "North", "Stay"};
    return out << desc[(int)d];
}

extern Direction directions[4];


#endif // DirectionExt_h__
