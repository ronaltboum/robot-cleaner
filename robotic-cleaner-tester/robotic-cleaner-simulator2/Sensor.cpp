//#include "stdafx.h"
#include "Sensor.h"

Sensor::Sensor(void)
	: _currentHouse(NULL), _currentPosition(NULL){ }

Sensor::Sensor(const House * housePtr, const Point * pointPtr)
	: _currentHouse(housePtr), _currentPosition(pointPtr){ }

// Brief: returns SensorInformation about the current location of the robot
SensorInformation Sensor::sense() const
{
	assert(_currentPosition != NULL && _currentHouse != NULL);
	Point pointsAfterMoving[4] = {*_currentPosition, *_currentPosition, *_currentPosition, *_currentPosition};
	SensorInformation returnedInfo;
	returnedInfo.dirtLevel = _currentHouse->GetDirtLevel(*_currentPosition);
	for(unsigned int i=0; i< 4; i++)
	{
		bool canMove = pointsAfterMoving[i].Move(Direction(i));
		if( ! canMove)
		{
			returnedInfo.isWall[i] = true;
			continue;
		}
		bool isInsideTheHouse = _currentHouse->IsPositionValid(pointsAfterMoving[i]);
		if(! isInsideTheHouse)
		{
			returnedInfo.isWall[i] = true;
			continue;
		}
		returnedInfo.isWall[i] = _currentHouse->IsWall(pointsAfterMoving[i]);
	}
	return returnedInfo;
}


unsigned int Sensor::GetCurrentRow()
{
	unsigned int currentRow =  _currentPosition->GetRow();
	return currentRow;
}

unsigned int Sensor::GetCurrrentCol()
{
	unsigned int currentCol =  _currentPosition->GetCol();
	return currentCol;
}

