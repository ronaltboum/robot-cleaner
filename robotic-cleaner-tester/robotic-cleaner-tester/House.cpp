#include "House.h"
#include <iostream>       //ron added
using namespace std;      //ron added

namespace ns_robotic_cleaner
{
	House::House(const int houseWidth, const int houseHeight)
	{
		houseFloor = Matrix(houseWidth, houseHeight);
	}

	//************************************
	// Brief:		Gets a location and returns the dirt level in it
	// Gets:	 	const Point & location
	// Returns:   	int 0-9
	// Access:    	public 
	// Pre:			-
	// Post:		-
	//************************************
	int House::GetDirtLevel(const Point & location) const
	{
		char tileContent = houseFloor(location);
		if(IsRegularTile(tileContent))
			return CharToNum(tileContent);
		return 0;
	}

	bool House::IsHouseClean() const
	{
		int h=0, int w=0;
		char tileContent;
		for(;h<height;++h){
			for(;w<width;++w){
				tileContent = houseFloor(w,h);
				if (IsDirty(tileContent))
				{
					return false;
				}
			}
		}

		return true;
	}

} // end of namespace ns_robotic_cleaner



