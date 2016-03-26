#include "stdafx.h"
#include "House.h"

namespace ns_robotic_cleaner
{
	House::House(const unsigned int houseWidth, const unsigned int houseHeight)
	{
		floor = Matrix<char>(houseWidth, houseHeight);
	}

	//************************************
	// Brief:		Gets a location and returns the dirt level in it
	// Returns:   	int 0-9
	// Pre:			IspositionValid(position)
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

	//int _tmain(int argc, _TCHAR* argv[])
	//{
	//	
	//	std::cout << "main method";
	//	getchar();
	//	return 0;
	//}

} // end of namespace ns_robotic_cleaner



