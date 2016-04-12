//#include "stdafx.h"
#include "RandomRobotAlgorithm.h"

namespace ns_robotic_cleaner_simulator
{



	RandomRobotAlgorithm::RandomRobotAlgorithm(const AbstractSensor& sensor, map<string, int> config)
	{
		setSensor(sensor);
		setConfiguration(config);
	}

	Direction RandomRobotAlgorithm::step()
	{
		SensorInformation info = robotSensor->sense();
		vector<Direction> possibleDirections;
		for(int i=0;i<4;++i){
			if( ! info.isWall[i])
				possibleDirections.push_back(Direction(i));
		}
		possibleDirections.push_back(Direction(4) );                        //just changed
		int chosenDirectionIndex = rand() % possibleDirections.size();
		return possibleDirections.at(chosenDirectionIndex);
	}

	extern "C" {
		AbstractAlgorithm * maker(){
			return new RandomRobotAlgorithm;
		}
		class proxy { 
		public:
			proxy(){
				// register the maker with the factory 
				factory["AbstractAlgorithm"] = maker;
			}
		};



} // end of namespace ns_robotic_cleaner_simulator_simulator