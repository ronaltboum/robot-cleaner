#include "stdafx.h"
#include "Simulator.h"


namespace ns_robotic_cleaner_simulator
{
	Simulator::Simulator(void)
	{
		houses.push_back(House());
	}
	
	
	Simulator::~Simulator(void)
	{
	}

	void Simulator::SimulateAll()
	{
		houses.at(0).Print();

	}

} //end of namespace ns_robotic_cleaner