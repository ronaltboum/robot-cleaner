//#include "stdafx.h"
#include "SubSimulation.h"

// SubSimulation::SubSimulation(void)
// 	: _singleRuns(NULL){_doesWinnerExist = false;}

SubSimulation::SubSimulation(vector< AlgorithmSingleRun *> singleRuns)
	: _singleRuns(singleRuns){ _doesWinnerExist = false; }
	
	
SubSimulation::~SubSimulation(void)
{
  
  //vector< AlgorithmSingleRun *> _singleRuns;
  for (vector<AlgorithmSingleRun *>::iterator j = _singleRuns.begin(); j != _singleRuns.end(); ++j) {
		delete *j;
  }
  _singleRuns.clear();
}