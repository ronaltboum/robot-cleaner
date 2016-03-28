#include "stdafx.h"
#include "AlgorithmSingleRun.h"


namespace ns_robotic_cleaner_simulator
{
	
	AlgorithmSingleRun::AlgorithmSingleRun(void)
		:_currentHouse(*(new House()))/*, _currentAlgorithm(RandomRobotAlgorithm()) */ 
	{
	}
	
	
	AlgorithmSingleRun::~AlgorithmSingleRun(void)
	{
	}
}
