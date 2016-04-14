#include "House.h"
#include "SensorInformation.h"
#include <assert.h>
#ifndef DynamicHouse_h__
#define DynamicHouse_h__

/*
 * class DynamicHouse is a class representing how the algorithm sees the house - 
 * A rectangle with unexplored areas that might be bigger and expand
 */
class DynamicHouse : House
{
//~~~~~~~~~~~~~~~~~~~~` Macros ~~~~~~~~~~~~~~~~~~~~~~~~~`
#define d_cUnexploredLetter 'U'
#define d_cReacableLetter 'R' // used to represent a tile which we can get data about it from neighbor squares

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	DynamicHouse(void);
	~DynamicHouse(void);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	bool IsValidTile(char tile) const; //: check that a tile is valid
	//void UpdateHouse(Point p, SensorInformation info); //: gets a point and the sensor information from it and updates the house
}; //end of class DynamicHouse


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~
inline bool DynamicHouse::IsValidTile(char tile) const
{
	return ( string("DWU 123456789").find(tile) != string::npos); 
}



#endif // DynamicHouse_h__

