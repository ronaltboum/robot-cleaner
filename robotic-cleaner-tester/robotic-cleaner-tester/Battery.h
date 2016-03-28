#ifndef __BATTERY__H_
#define __BATTERY__H_

#include "stdafx.h"

namespace ns_robotic_cleaner
{

	class Battery
	{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		private:
			unsigned int _battery_capacity;
			unsigned int _recharge_rate;
			unsigned int _consumption_rate;
			int _battery_level;  //initialized to _battery_capacity

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		public:
			Battery(unsigned int battery_capacity, unsigned int recharge_rate, unsigned int consumption_rate);
	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
		unsigned int GetBattery_capacity() const;
		void SetBattery_capacity(unsigned int battery_capacity);  
		unsigned int GetRecharge_rate() const;
		void SetRecharge_rate(unsigned int recharge_rate);
		unsigned int GetConsumption_rate() const;
		void SetConsumption_rate(unsigned int consumption_rate);
		int GetBattery_level() const;  //returns a number  <=  _battery_capacity. set func is Consume()

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//functions below are implemented in Battery.cpp
		//bool IsEmpty();  //returns true iff battery is empty
		bool Consume();		//eats the battery _consumption_rate at each time unit the robot isn't in
		//it's docking point.  returns false if _battery_level < 0 after consume - game over
		void Recharge();	//fill the battery with _recharge_rate at each time unit the robot is in
		//it's docking point

	};

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

		inline Battery::Battery(unsigned int battery_capacity = 0, unsigned int recharge_rate = 0, 
			unsigned int consumption_rate = 0) : _battery_capacity(battery_capacity),
			_recharge_rate(recharge_rate), _consumption_rate(consumption_rate)
		{
			_battery_level = _battery_capacity;
		}

		inline unsigned int Battery::GetBattery_capacity() const
		{
			return _battery_capacity;
		}

		inline void Battery::SetBattery_capacity(unsigned int battery_capacity)
		{
			_battery_capacity = battery_capacity;
		}

		inline unsigned int Battery::GetRecharge_rate() const
		{
			return _recharge_rate;
		}

		inline void Battery::SetRecharge_rate(unsigned int recharge_rate)
		{
			_recharge_rate = recharge_rate;
		}

		inline unsigned int Battery::GetConsumption_rate() const
		{
			return _consumption_rate;
		}

		inline void Battery::SetConsumption_rate(unsigned int consumption_rate)
		{
			_consumption_rate = consumption_rate;
		}

		inline int Battery::GetBattery_level() const
		{
			return _battery_level;
		}


} // end of namespace ns_robotic_cleaner

#endif //__BATTERY__H_