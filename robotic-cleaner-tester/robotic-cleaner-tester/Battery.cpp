#include "stdafx.h"
#include "Battery.h"
#include <string>
#include <map>

namespace ns_robotic_cleaner_simulator
{

	Battery::Battery(const map<string,int> & configs)
	{
		auto iter = configs.find("BatteryCapacity");
		if (iter != configs.end())
			_battery_capacity = iter->second;
		iter = configs.find("BatteryConsumptionRate");
		if (iter != configs.end())
			_consumption_rate = iter->second;
		iter = configs.find("BatteryRachargeRate");
		if (iter != configs.end())
			_recharge_rate = iter->second;
		_battery_level = _battery_capacity;
	}

	Battery::Battery(const Battery & other)
	{
		_battery_capacity = other._battery_capacity;
		_battery_level = other._battery_level;
		_consumption_rate = other._consumption_rate;
		_recharge_rate = other._recharge_rate;
	}

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
		if(_battery_level > _battery_capacity)
			_battery_level = _battery_capacity;
	}

	void Battery::initialize()
	{
		_battery_capacity = 0;
		_consumption_rate = 0;
		_recharge_rate = 0;
		_battery_level = _battery_capacity;
	}

} // end of namespace ns_robotic_cleaner_simulator