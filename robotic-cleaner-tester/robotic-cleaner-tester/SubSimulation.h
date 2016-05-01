#ifndef SubSimulation_h__
#define SubSimulation_h__

#include "Direction.h"
#include "AlgorithmSingleRun.h"
#include <string>

using namespace std;

class SubSimulation
{

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Members ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	vector< AlgorithmSingleRun *> _singleRuns;
	bool _doesWinnerExist;
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ctor/Dtor ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
public:
	SubSimulation(void); 
	SubSimulation(vector< AlgorithmSingleRun *> singleRuns);
	
	~SubSimulation(void);
	
	
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Getters/Setters ~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool GetDoesWinnerExist() const;
	vector< AlgorithmSingleRun *> GetSingleRuns();
	void SetDoesWinnerExist(bool b);
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ inline functions ~~~~~~~~~~~~~~~~~~~~~~~~~~

inline bool SubSimulation::GetDoesWinnerExist() const
{
	return _doesWinnerExist;
}

inline void SubSimulation::SetDoesWinnerExist(bool b)
{
	 _doesWinnerExist = b;
}

inline vector< AlgorithmSingleRun *> SubSimulation::GetSingleRuns()
{
	 return _singleRuns;
}


#endif // SubSimulation_h__