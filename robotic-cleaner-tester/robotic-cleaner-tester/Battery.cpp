#include "stdafx.h"
#include "Battery.h"

namespace ns_robotic_cleaner_simulator
{

	bool Battery::Consume() //eats the battery _consumption_rate at each time unit the robot isn't in
		//it's docking point. returns false if _battery_level < 0 after consume - game over
	{
		int consumptionRate = GetConsumption_rate();
		_battery_level = _battery_level - consumptionRate;
		if(_battery_level < 0)
		{
			return false;  //robot failed to clean house.  stop single run

			//should handle case where _battery_level == 0 and we are not in docking point. but 
			//we'll handle it outside this class
		}
		return true;
	}


	void Battery::Recharge()
	{
		_battery_level = _battery_level + _recharge_rate;
	}
} // end of namespace ns_robotic_cleaner