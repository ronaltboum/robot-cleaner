#include "stdafx.h"
#include "RandomRobotAlgorithm.h"

namespace ns_robotic_cleaner_simulator
{

	vector< Point > RandomRobotAlgorithm::getPossibleDirections( const Point& currentLocation) 
	//returns the legal points we can go to, i.e. points which aren't walls or outside the house
	{
		int currentRow = ((Sensor)robotSensor)->GetCurrentRow();
		vector<Point>  possibleDirections;
		SensorInformation info = robotSensor.sense();
		//first we push currentPoint to vector<Point> to represent the stay option
		//Point current = robotSensor._currentPosition;
		 //possibleDirections.push_back(pair<int, int>(iNew, jNew));
		//Point neighborPoint = Point(*_currentPosition);
		for (int i=0; i<4; i++)
		{
			if (info.isWall[i] == false)  //add point to possibleDirections
			{
				//Point nighborPoint = Point()
			}
		}
	}


} // end of namespace ns_robotic_cleaner