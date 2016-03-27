#ifndef __BATTERY__H_
#define __BATTERY__H_

#include "stdafx.h"

namespace ns_robotic_cleaner
{

	class Battery
	{
		private:
			unsigned int _battery_capacity;
			unsigned int _recharge_rate;
			unsigned int _consumption_rate;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		public:
			Battery(unsigned int battery_capacity, unsigned int recharge_rate, unsigned int consumption_rate);
	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~

	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

		inline Battery::Battery(unsigned int battery_capacity = 0, unsigned int recharge_rate = 0, 
			unsigned int consumption_rate = 0) : _battery_capacity(battery_capacity),
			_recharge_rate(recharge_rate), _consumption_rate(consumption_rate)
		{
		}

//Class Battery
//	members
//	int batteryLevel.  changed name to battery_capacity
//	int rechargeRate
//	int consumptionRate
//functions:
//bool IsEmpty()
//	GetBatteryLevel()
//	void Recharge()		//fill the battery
//	void Consume()		//eats the battery





} // end of namespace ns_robotic_cleaner

#endif //__BATTERY__H_