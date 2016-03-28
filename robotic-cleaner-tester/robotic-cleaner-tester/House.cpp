#include "stdafx.h"
#include "House.h"
#include <iostream>

namespace ns_robotic_cleaner
{
	House::House(){
		floor = Matrix<char>(19,81);
		char house[19][81] = {
		//             1         2         3         4         5         6         7        
		//   01234567890123456789012345678901234567890123456789012345678901234567890123456789
			"W123WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
			"W  79   D              1234321                                                 W", // 1
			"W  98      WWWWWWW     1234321                     W                       1   W", // 2
			"W              W                                   W   555                 2   W", // 3
			"W              W                                   W   555                 3   W", // 4
			"W              W           WWWWWWWWWWWWWWWWWWWWWWWWW                       4   W", // 5
			"W              W                                                           5   W", // 6
			"W              W                                                           6   W", // 7
			"W                          WWWWWWWWWWWWWWWWWWWWWW  WWWWWWW                 7   W", // 8
			"W         1         2         3         4         5W 999 W  6         7        W", // 9
			"W              W           444                     W 999 W                 9   W", // 10
			"W              W           444                     W 999 W                 8   W", // 11
			"W              W                                   W     W                 7   W", // 12
			"W              W                                   WW   WW                 6   W", // 13
			"W              W                                    W   W                  5   W", // 14
			"W              W                                                           4   W", // 15
			"W              W                                                           3   W", // 16
			"W              W                                                               W", // 17
			"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" // 18
		};
		for(unsigned int row = 0; row < 19; ++row) {
			for(unsigned int col = 0; col < 81; ++col) {
				floor(row,col) = house[row][col];
			}
		}
	}

	House::House(const unsigned int houseWidth, const unsigned int houseHeight)
	{
		floor = Matrix<char>(houseWidth, houseHeight);
	}

	//************************************
	// Brief:		Gets a location and returns the dirt level in it
	// Returns:   	int 0-9
	// Pre:			IsPositionValid(position)
	//************************************
	unsigned int House::GetDirtLevel(const Point & location) const
	{
		char tileContent = floor(location);
		if(IsRegularTile(tileContent))
			return CharToNum(tileContent);
		return 0;
	}

	// Brief:		Check if all the tiles in the house are clean
	bool House::IsHouseClean() const
	{
		unsigned int hIterator=0, wIterator=0;
		unsigned int height = GetHeight(), width = GetWidth();
		char tileContent;
		for(;hIterator<height;++hIterator){
			for(;wIterator<width;++wIterator){
				tileContent = floor(wIterator,hIterator);
				if (IsDirty(tileContent))
				{
					return false;
				}
			}
		}
		return true;
	}

	void House::PrintRow(int row) const
	{
		assert(row < GetHeight() && row >= 0);
		unsigned int rows = GetHeight();
		unsigned int cols = GetWidth();
		for(unsigned int col = 0; col < cols; ++col) {
			cout << floor(row,col);
		}
	}

	void House::Print() const
	{
		unsigned int rows = GetHeight();
		unsigned int cols = GetWidth();
		char c;
		for(unsigned int row = 0; row < rows; ++row) {
			PrintRow(row);
			cout << endl;
		}
	}


} // end of namespace ns_robotic_cleaner



