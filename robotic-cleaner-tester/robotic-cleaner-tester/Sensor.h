#ifndef Sensor_h__
#define Sensor_h__

#include "AbstractSensor.h"
#include "House.h"


/*!
* \class Sensor
* class Sensor is a class implementing the robot sensor
*/
class Sensor : public AbstractSensor
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	const House * _currentHouse;
	const Point * _currentPosition;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	Sensor(void);
	Sensor( const House * housePtr, const Point * pointPtr);
	~Sensor(void) {};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SensorInformation sense() const; //: returns SensorInformation about the current location of the robot
	unsigned int GetCurrentRow(); //returns row number of _currentPosition
	unsigned int GetCurrrentCol();  //returns column number of _currentPosition
	const Point * GetCurrentPosition() const {return _currentPosition;}
	
	//void SetSensor(const House * housePtr, const Point * pointPtr);

};

#endif // Sensor_h__
