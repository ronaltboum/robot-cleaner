#ifndef DirectionExt_h__
#define DirectionExt_h__

#include "Direction.h"
#include <ostream>
#include <string>
#include <vector>

using namespace std;

// prefix++ for Direction, skipping Stay
inline Direction operator++(Direction& d) {
    d = (Direction)(((int)d + 1)%4);
    return d;
}

// added for debug purposes
inline string DirectionToString(Direction d) {
    static const char* desc[] = {"East", "West", "South", "North", "Stay"};
    return desc[(int)d];
}

inline string DirectionsToString(vector<Direction> dirs) {
	string s;
	for(auto d : dirs)
		s = s + DirectionToString(d) + ",";
	return s;
}

// added for debug purposes
inline ostream& operator<<(ostream& out, Direction d) {
    return out << DirectionToString(d);
}

inline Direction OppositeDirection(Direction d){
	switch (d)
	{
	case Direction::North:
		return Direction::South;
	case Direction::South:
		return Direction::North;
	case Direction::East:
		return Direction::West;
	case Direction::West:
		return Direction::East;
	default:
		return Direction::Stay;
	}
}

extern Direction directions[4];


#endif // DirectionExt_h__
