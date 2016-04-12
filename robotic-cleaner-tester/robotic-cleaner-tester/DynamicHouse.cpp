#include "stdafx.h"
#include "DynamicHouse.h"
namespace ns_robotic_cleaner_simulator
{

DynamicHouse::DynamicHouse(void)
{
	_floor = Matrix<char>(3, 3, d_cUnexploredLetter);
	_floor(1,1) = d_cDockingLetter;
}


DynamicHouse::~DynamicHouse(void)
{
}
////************************************
//// Brief:		Gets a point and the sensor information from it and updates the house
//// Gets:	 	Point p - from where information was extracted
//// Gets:	 	SensorInformation i
//// Post:		the house floor updated
////************************************
//void DynamicHouse::UpdateHouse(Point p, SensorInformation info)
//{
//	char tileContent = info.dirtLevel ? '0' + info.dirtLevel : ' ';
//	bool b= IsPositionValid(p);
//	assert(b);
//	SetTile(p,tileContent);
//	Point neighbors[4] = {p};
//	bool shouldExpand[4] = {false};
//	for(unsigned int i=0; i< 4; i++)
//	{
//		neighbors[i] = neighbors[i].Move(Direction(i));
//		if( ! info.isWall[i] )
//		{
//			SetTile(neighbors[i], d_cWallLetter);
//			continue;
//		}
//		bool isInsideTheHouse = _currentHouse->IsPositionValid(pointsAfterMoving[i]);
//		if(! isInsideTheHouse)
//		{
//			returnedInfo.isWall[i] = true;
//			continue;
//		}
//		returnedInfo.isWall[i] = _currentHouse->IsWall(pointsAfterMoving[i]);
//	}
//	return returnedInfo;
//	
//
//}

} //end of namespace ns_robotic_cleaner_simulator