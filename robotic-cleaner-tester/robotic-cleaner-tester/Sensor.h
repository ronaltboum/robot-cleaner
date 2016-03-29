#ifndef Sensor_h__
#define Sensor_h__

//#include "abstractsensor.h"  //shouldn't it be #include "AbstractSensor.h"   ?
#include "AbstractSensor.h"
#include "House.h"
//#include "Point.h"   //don't need it because House.h includes Point.h
//#include "Direction.h"  //don't need it because Point.h includes Direction.h
#include "SensorInformation.h"
namespace ns_robotic_cleaner_simulator
{


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
	~Sensor(void) override{} 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	SensorInformation Sense(); //: returns SensorInformation about the current location of the robot
	unsigned int GetCurrentRow(); //returns row number of _currentPosition
	unsigned int GetCurrrentCol();  //returns column number of _currentPosition

};


} // end of namespace ns_robotic_cleaner_simulator

#endif // Sensor_h__
