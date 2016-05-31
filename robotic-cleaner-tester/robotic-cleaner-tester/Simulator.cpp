//#include "stdafx.h"
#include "Simulator.h"
#include <string>
#include <math.h> 
#include <sstream>
using namespace std;

Simulator::Simulator(void)
{
  initiallize();
}



//************************************
// Brief:		called in each c'tor to initialize all
//************************************
void Simulator::initiallize()
{
	_houses = vector<House *>();  
	_configs = map<string, int>();
	_defaultBattery = NULL;
	_badHousesMap = map<string, string>();
	_scoresMap = map<string, map<string, int> >();
	_subSimulations =  vector< SubSimulation *>();
	_sortedAlgorithmsMap = map<double, vector<string>, std::greater<int>>();
	
}

Simulator::~Simulator(void)
{
  
	for (vector<House *>::iterator i = _houses.begin(); i != _houses.end(); ++i) {
		delete *i;
	}
	_houses.clear();
	for (vector< SubSimulation *>::iterator j = _subSimulations.begin(); j != _subSimulations.end(); ++j) {
		delete *j;
	}
	_subSimulations.clear();
	
	
	delete _defaultBattery;
	
}

bool Simulator::ReadConfigFile(const string & configFilePath){
	 
	bool valid = SingletonConfigReader::instance(configFilePath) -> IsConfigValid ();
	
	//cout << "in Simulator::ReadConfigFile  valid config == " << valid << endl;

	if(valid == false)
	    return false;

	string path = configFilePath;
	if(configFilePath != "config.ini")
		path = path + "config.ini";

	bool missingParameters = SingletonConfigReader::instance(path) -> ReadConfigFromFile();
	if(missingParameters == false)
	    return false;
	
	_configs = SingletonConfigReader::instance(path) -> GetConfigMap();
	_defaultBattery = new Battery(_configs);

// 	map<string,int>::iterator strIt;
// 	string bat = "BatteryRechargeRate";
// 	strIt = _configs.find(bat);
// 	cout << endl << endl << "BatteryRechargeRate = " << (strIt->second) << endl<< endl;
	
	return missingParameters;
}


//************************************
// Brief:		Gets a vector<string> with house files and loads them to memory. Inserts bad houses
//              and their error messages to _badHousesMap
// Gets:	 	char * houseFolder - string of the folder path
// Returns:   	int - the number of houses loaded successfully
// Post:		_houses
//************************************
int Simulator::LoadHouses( vector<string> housesVector )
{
	int validHouses = 0;
	std::string isValidAnswer, houseFileName;
	vector<string>::iterator _stringIterator;
	for(_stringIterator = housesVector.begin() ; _stringIterator!=housesVector.end() ; _stringIterator++){
		//House * h = SingletonHouseIOManager::instance()->ReadHouseFromFile("house1.house");
		houseFileName = *_stringIterator;
		House * h = SingletonHouseIOManager::instance()->ReadHouseFromFile(houseFileName);
		string noSuffix = SingletonHouseIOManager::ExtractHouseName( houseFileName);
		string withSuffix = noSuffix + ".house";
		
		if(h == NULL){
			_badHousesMap.insert( std::pair<std::string, std::string>(withSuffix, "cannot open file") );
			//delete h;
			//h = NULL;
			continue;
		}
		isValidAnswer = h->isValid();

		if(isValidAnswer.compare("valid") == 0 ){
			++validHouses;
			_houses.push_back(h);
		
		} else { 
			_badHousesMap.insert( std::pair<std::string, std::string>(withSuffix, isValidAnswer) );
			delete h;
			h = NULL;
		}
	
	} //closes for loop
	
	return validHouses;	
}


//************************************
// Brief:		Load algorithms from libraries
//				
// Returns:   	int - the number of algorithm loaded successfully
// Post:		_algorithms
//************************************
int Simulator::LoadAlgorithms(vector<string> algorithmFiles)
{
	int validAlgorithms = AlgorithmFactory::getInstance().ReadAlgorithms(algorithmFiles);
	return validAlgorithms;
}



int Simulator::LoadRuns()
{
	int houseNum = _houses.size();  //number of valid houses
	vector<AbstractAlgorithm *> algorithms;
	vector<AbstractAlgorithm *>::iterator _algoIterator;
	
	vector<string> algoNames = AlgorithmFactory::getInstance().GetAlgorithmNames();
	vector<string>::iterator algoNamesIterator;
	
	for( int houseIndex = 0 ; houseIndex < houseNum; ++houseIndex)
	{
		
		algorithms = AlgorithmFactory::getInstance().CreateSetOfAllAlgorithms();
	
		vector< AlgorithmSingleRun *> singleRuns;
		algoNamesIterator = algoNames.begin();
		for(_algoIterator=algorithms.begin() ; _algoIterator!=algorithms.end() ; _algoIterator++){
			Point * startingPoint = _houses[houseIndex]->GetDockingStation(); //new is deallocated by AlgorithmSingleRun

			if(debugSimulator || false) {
				int pRow = startingPoint -> GetRow();
 				int pCol = startingPoint -> GetCol();
 				cout<< "In Simulator:  startingPoint = " << pRow << " , " << pCol << endl << endl;  //delte !!!!!!!!!!
				_houses[houseIndex]->Print();
				cout << endl << endl;
			}


			int width = ( _houses[houseIndex])-> GetWidth();
			int height = ( _houses[houseIndex])-> GetHeight();
			int max = ( _houses[houseIndex])-> GetMaxSteps();
			vector<char> inner = ( ( _houses[houseIndex])-> GetFloor() ).GetInner();
			string errorMessage = ( _houses[houseIndex])-> GetErrorMessage();
			string hFileName = ( _houses[houseIndex])-> GetHouseFileName();

			House * houseCopy = new House(width, height, inner, max, errorMessage, hFileName);
			Sensor * algoSensor = new Sensor(houseCopy,startingPoint); // new is deallocated by AlgorithmSingleRun 
 					
			(*_algoIterator)->setConfiguration(_configs);
			(*_algoIterator)->setSensor(*algoSensor);
			//algorithms was created  in the same order as  AlgorithmFactory -> algorithmNames
			string algoName = (*algoNamesIterator);
			
			AlgorithmSingleRun * newRunCreated = new AlgorithmSingleRun(_configs, (*_algoIterator), *_defaultBattery, houseCopy ,algoSensor, startingPoint, algoName);
			
 			singleRuns.push_back(newRunCreated);  
 			++algoNamesIterator;
		}
		
		SubSimulation * newSub = new SubSimulation(singleRuns); 
		_subSimulations.push_back(newSub);
	}
	return 1;  
}

//pre: num_threads >= 1
void Simulator::RunAllHouses(size_t num_threads)
{
  int houseNum = _houses.size();  //number of valid houses
  if(num_threads == 1){
    for( int houseIndex = 0 ; houseIndex < houseNum; ++houseIndex)
    {
      RunAll(houseIndex);
    }
  }
  else {   //num_threads > 1
    
//     // ===> create the threads as vector of pointers to threads (unique_ptr)
//         vector<unique_ptr<thread>> threads(num_threads);
//         for(auto& thread_ptr : threads) {
//             // ===> actually create the threads and run them
//             thread_ptr = make_unique<thread>(&Simulation::runSingleSubSimulationThread, this); // create and run the thread
//         }
//         
//         // ===> join all the threads to finish nicely (i.e. without crashing / terminating threads)
//         for(auto& thread_ptr : threads) {
//             thread_ptr->join();
//         }
  

    //create the threads and run them:
    vector<std::thread> vecThread(num_threads);
    for( size_t thNum = 0 ; thNum < num_threads; ++thNum)
    {
      vecThread.at(thNum) = std::thread(&Simulator::RunSingleSubSimulationThread, this);
     // vecThread.at(thNum) = std::thread(&Simulator::Foo, this);
    }
    
    //wait for all threads to finish
    for( size_t thNum = 0 ; thNum < num_threads; ++thNum)
    {
      vecThread.at(thNum).join();
    }
      
  }
  
}


void Simulator::RunSingleSubSimulationThread()
{
        // ===> thread should take a new task, if available, and run it
        // if no task is available, thread is done
        for(size_t index = _indexOfHouse++; // fetch old value, then add. equivalent to: fetch_add(1)
            index < _houses.size();
            index = _indexOfHouse++) {
	  
	  
		cout << std::this_thread::get_id() << ": " << "is assigned to house at index: " << index << endl;  //delete !!!!!!!!!!!!!!!!!!
                RunAll(index); //runs all algorithms on the house in _houses[houseIndex]
        }
        return;
}

//Brief: runs all algorithms on the house in _houses[houseIndex] in a round robin fashion
void Simulator::RunAll(int houseIndex)
{
  
	//cout << std::this_thread::get_id() << ": " << "is in RunAll on house at index: " << houseIndex << endl;  //delete !!!!!!!!!!!!!!!!!!
  
  
	SubSimulation * sub = _subSimulations[houseIndex]; 
	int maxSteps = _houses[houseIndex] -> GetMaxSteps();
	bool isThereAWinner = sub -> GetDoesWinnerExist();
	int currentStep = 0;   int simulationSteps = 0;
	int currentRankAlgorithmsCompetingOn = 1;
	bool finishEarly = false;  //finishEarly == true iff all algorithms finished before maxSteps. This happens when: 1.all won  2.all hit wall   3.all ran out of battery
	//run until someone one or sombodywon

	for(;currentStep < maxSteps && ! isThereAWinner && (finishEarly == false); ++currentStep){
		finishEarly = MoveAllOneStep(currentRankAlgorithmsCompetingOn, houseIndex);
		isThereAWinner = sub -> GetDoesWinnerExist();	
	}
	
	simulationSteps = currentStep;
	if(finishEarly == true){
	  registerScores(currentStep, houseIndex, simulationSteps); //in case there is no winner, then winner_num_steps == simulationSteps.  see http://moodle.tau.ac.il/mod/page/view.php?id=374508
	  //cout << "finishi early"<<endl; // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!
	  return;
	}
	
	//move all if possible after winning
	int winnerStepsNumber = currentStep;
	int maxStepsAfterWinner = _configs["MaxStepsAfterWinner"];
	for(;currentStep < maxSteps  && currentStep < winnerStepsNumber + maxStepsAfterWinner ; ++currentStep){
		MoveAllOneStep(currentRankAlgorithmsCompetingOn, houseIndex);
	}

	//cout << "printing scores for house index: " << houseIndex << endl;
	simulationSteps = currentStep;
	registerScores(winnerStepsNumber, houseIndex, simulationSteps);
		
}

bool Simulator::MoveAllOneStep(int & currentRankAlgorithmsCompetingOn, int houseIndex)
{
	SubSimulation * sub = _subSimulations[houseIndex]; 
	vector< AlgorithmSingleRun *> singleRuns = sub -> GetSingleRuns();
	int runNum = singleRuns.size(); 
	int numAlgorithmWon = 0; //for this step
	int algosRunning = 0;
	int maxSteps = _houses[houseIndex] -> GetMaxSteps();
	vector<Direction> lastSteps(runNum, Direction::Stay);
	bool alreadyNotified = false;  
	for( int runIndex = 0 ; runIndex < runNum; ++runIndex)
	{
		AlgorithmSingleRun * runIterator = singleRuns[runIndex];
		if(runIterator->CanDoStep()){
			++algosRunning;
			lastSteps[runIndex] = runIterator-> GetLastDirection(); 

			//cout << "in Simulator before DoStep:  lastSteps[runIndex] =";	  //delete!!!!!!!!!!!!!!!!!!!!!
			//PrintDirection(lastSteps[runIndex]); //delete!!!!!!!!!!!!!!!!!!!!!	
			lastSteps[runIndex] = runIterator->DoStep(lastSteps[runIndex]); 
			//cout << "in Simulator: after DoStep:  lastSteps[runIndex] =";			
			//PrintDirection(lastSteps[runIndex]); //delete!!!!!!!!!!!!!!!!!!!!!
	
			if(runIterator->HasWon()){
				runIterator->SetActualPosition(currentRankAlgorithmsCompetingOn);
				++numAlgorithmWon;
				sub -> SetDoesWinnerExist(true);
				//notify all algorithms running on this house, that there's a winner:
				CallAboutToFinish(houseIndex);				
	
				//cout << "win!!!!!!!!!!!!!!!"<<endl;  //delete!!!!!!!!!!!!!!!!!!!!!
			}
		}
		//call aboutToFinish when steps == MaxSteps - MaxStepsAfterWinner
		if(runIterator -> GetNumberOfStepsCommited()  == (maxSteps - GetMaxStepsAfterWinner() )  ) {
			if(alreadyNotified == false)
				CallAboutToFinish(houseIndex);
			alreadyNotified = true;  //we only need to notify once per house	
		}


		//debug
		if(debugSimulator) {
			cout << "numOfStepsCommited = " << runIterator -> GetNumberOfStepsCommited() << endl;
			//cout << "maxSteps = " << maxSteps << endl;
			//cout << "MaxStepsAfterWinner = " << GetMaxStepsAfterWinner() << endl;
		}
	}
	currentRankAlgorithmsCompetingOn += numAlgorithmWon;
	if(algosRunning == 0)
	    return true;
	return false;
}


//call aboutToFinish for all algorithms running ho house at index houseIndex:
void Simulator::CallAboutToFinish(int houseIndex)
{
	SubSimulation * sub = _subSimulations[houseIndex]; 
	vector< AlgorithmSingleRun *> singleRuns = sub -> GetSingleRuns();
	for (size_t runIndex = 0; runIndex < singleRuns.size()  ; runIndex++) {
		AlgorithmSingleRun * runIterator = singleRuns[runIndex];
		(runIterator -> GetCurrentAlgorithm() ) -> aboutToFinish( GetMaxStepsAfterWinner() );
	}
}


int Simulator::GetMaxStepsAfterWinner() const
{
	//map<string, int> _configs
	map<string, int>::const_iterator it;
	it = _configs.find("MaxStepsAfterWinner");
	 if (it != _configs.end())
		return it->second;
	return 0; //should never get here
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Print Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// winner_num_steps == maxSteps if nobody won
void Simulator::registerScores(int winner_num_steps, int houseIndex, int simulationSteps)
{	
	SubSimulation * sub = _subSimulations[houseIndex]; 
	vector< AlgorithmSingleRun *> singleRuns = sub -> GetSingleRuns();
	int runNum = singleRuns.size();
	
	//cout << "number of algo in SubSimulation = " << runNum << endl;
	
	//int runNum = _runs.size();
	string hNameNoSuffix, algoName;
	int this_num_steps;
	for( int runIndex = 0 ; runIndex < runNum; ++runIndex)
	{
		AlgorithmSingleRun * runIterator = singleRuns[runIndex];
		algoName = runIterator-> GetAlgorithmFileName(); 
		hNameNoSuffix =  _houses[houseIndex] -> GetHouseFileName();
		this_num_steps = runIterator->GetNumberOfStepsCommited();

		if(runIterator->HasMadeIllegalStep())  {   //algorithm hit a wall !
			//http://moodle.tau.ac.il/mod/forum/discuss.php?d=57047
			//Algorithm 331332334_E_ when running on House 003 went on a wall in step 7
			string errorMessage = "Algorithm " + algoName + " when running on House " + hNameNoSuffix + " went on a wall in step " + to_string(this_num_steps);
			_hitWallErrorMessages.push_back(errorMessage);
			addScore(algoName, hNameNoSuffix, 0);
			continue;
		}
		bool hasWon = runIterator->HasWon();
		int actual_position = runIterator->GetActualPosition();
		int sum_dirt_in_house = runIterator->GetSumOfDirtBeforeCleaning();
		int dirt_collected = runIterator->GetDirtCollected();
		//if algorithm stopped because of empty battery, then the number of steps it committed will be simulationSteps. see:  http://moodle.tau.ac.il/mod/page/view.php?id=374508
		bool is_battery_empty = runIterator->IsAlgorithmBatteryEmpty();
		
		if(is_battery_empty == true && hasWon == false)
		    this_num_steps = simulationSteps;
		bool is_back_in_docking = runIterator->IsBackInDocking(); 

		map<string, int> params= {{"actual_position_in_competition", actual_position}, {"simulation_steps", simulationSteps}, {"winner_num_steps", winner_num_steps}, {"this_num_steps", this_num_steps}, {"sum_dirt_in_house", sum_dirt_in_house}, {"dirt_collected", dirt_collected}, {"is_back_in_docking", is_back_in_docking}};
		int score = ScoreRegistrar::getInstance().calc_score(params);
		if(score == -1)
			_scoreFormulaProblem = true;
		//string trimmedHouseName = hNameNoSuffix.substr(0,9);
		addScore(algoName, hNameNoSuffix, score);
		
		// cout << algoName <<" on house "<< hNameNoSuffix << " returned to docking == " << is_back_in_docking << endl; //delete later !!
		// cout << algoName <<" on house "<< hNameNoSuffix << " :  sum_dirt_in_house = " << sum_dirt_in_house << "  , dirt_collected = " << dirt_collected << endl;    //delete later!!!  

		// cout << algoName << "on house " << hNameNoSuffix << " this_num_steps = " << this_num_steps << endl;  //delete

		if(debugSimulator || false) {
				
 			
				runIterator -> GetCurrentHouse() -> Print();
				cout << endl << endl;
			}
	}
	
}

void Simulator::printScores()
{
	//cols = num of valid houses + 2
	int houseNum = _houses.size();  //number of valid houses
	int cols = houseNum + 2;
	//rows = num of valid  algos + 1
	//int rows = algoNum + 1;
	int numCharsInRow = 13 + 10*(cols - 1) + (cols + 1);
	printDashes(numCharsInRow);
	vector<string> trimmedValidHouseNames = GetValidTrimmedHousesNames();
	printFirstRow(houseNum, trimmedValidHouseNames);
	printDashes(numCharsInRow);
	vector<string> algoNames = AlgorithmFactory::getInstance().GetAlgorithmNames();
  

//  vector<string>::iterator algoNamesIt;
//  for(algoNamesIt = algoNames.begin() ; algoNamesIt != algoNames.end(); algoNamesIt++) {
//    string algoName = (*algoNamesIt);
//    printRow(algoName);
//    printDashes(numCharsInRow);
//  }
	
	SortAlgorithms(); 
	//map<int, vector<string>> _sortedAlgorithmsMap
	map<double, vector<string>>::iterator sortedIt;
	for(auto& a : _sortedAlgorithmsMap) {
		//if(a.second.dirt > 0) {
		for(int i=0; i < (int)a.second.size() ; i++) {
			printRow(a.second.at(i));
			printDashes(numCharsInRow);
		}
	}
  
}

//sorts algoritms by AVG
void Simulator::SortAlgorithms() 
{
	//map<int, vector<string>> _sortedAlgorithmsMap
	map<double, vector<string>>::iterator sortedIt;
	vector<string> algoNames = AlgorithmFactory::getInstance().GetAlgorithmNames();
	vector<string>::iterator algoNamesIt;
	for(algoNamesIt = algoNames.begin() ; algoNamesIt != algoNames.end(); algoNamesIt++) {
    	string algoName = (*algoNamesIt);
		vector<int> results = GetAlgoResults(algoName);  
	  	vector<int>::iterator intIt;
	  	int res = 0;   int sum = 0; 
	  	for(intIt = results.begin() ; intIt != results.end(); intIt++) {
			res = (*intIt);
			sum = sum + res;
		}
		int houseNum = _houses.size();  //number of valid houses
		double avg = double(sum)/ double(houseNum);
		double scale = 0.01;  // i.e. round to nearest one-hundreth
		avg = floor(avg / scale + 0.5) * scale;
		sortedIt = _sortedAlgorithmsMap.find(avg);
		if(sortedIt == _sortedAlgorithmsMap.end() ) {
			vector<string> currentAlgoName;
			currentAlgoName.push_back(algoName);
			_sortedAlgorithmsMap.insert( pair<double, vector<string>>(avg, currentAlgoName) );
		}
		else {
			//it -> second).insert ( pair<string,int>(trimmedHouseName ,score) );
//     _scoresMap[algoName] = (it -> second);
			(sortedIt -> second).push_back(algoName);
			_sortedAlgorithmsMap[avg] = (sortedIt -> second);
		}
	  	
	}
	
  
//// map<string, map<string, int> > _scoresMap     map<algo name, map<house name, int result> >
//  map< string, map<string, int> >::iterator it;
//  it = _scoresMap.find(algoName);
//  map<string, int> innerMap;
//  if ( it == _scoresMap.end() ) { // not found
//    innerMap.insert( pair<string, int>(trimmedHouseName ,score) );
//    _scoresMap.insert ( pair< string, map<string, int> >(algoName ,innerMap) );
//   
//  }else {  // found
//  
//    (it -> second).insert ( pair<string,int>(trimmedHouseName ,score) );
//     _scoresMap[algoName] = (it -> second);
//     
//     //cout << "algo = " << algoName << endl;
//     //cout << "house = " << trimmedHouseName << endl;
//  }
  

}


void Simulator::printRow(string algoName)
{

	if(algoName.length() <= 0)
		return;  //shouldn't get here
  //print algo name:
  cout << '|';  string trimmedAlgoName;
	if(algoName.at(0) == '/')
		trimmedAlgoName = algoName.substr(2,15);
	else
		 trimmedAlgoName = algoName.substr(1,14);

	//append _  :
	trimmedAlgoName = trimmedAlgoName + "_";
	
  cout << trimmedAlgoName;
  int algoNameLength = trimmedAlgoName.length();
  int quantity = 13- algoNameLength;
  printSpaces(quantity); 
  cout << '|';
  vector<int> results = GetAlgoResults(algoName);  
  vector<int>::iterator intIt;
  int res = 0;   int sum = 0; 
  for(intIt = results.begin() ; intIt != results.end(); intIt++) {
      res = (*intIt);
      sum = sum + res;
      string strRes = ConvertFromInt(res);
      int resLength = strRes.length();
      quantity = 10 - resLength;
      printSpaces(quantity); 
      cout << strRes;
      cout << '|';
  }
  int houseNum = _houses.size();  //number of valid houses
  double avg = double(sum)/ double(houseNum);
  double scale = 0.01;  // i.e. round to nearest one-hundreth
  avg = floor(avg / scale + 0.5) * scale;
  string strAvg = CovertFromDouble(avg);
  int avgLength = strAvg.length();
  quantity = 10 - avgLength;
  printSpaces(quantity); 
  cout << strAvg;
  cout << '|' << endl;
  
}

string Simulator::ConvertFromInt(int res)
{
  stringstream ss;
  ss << res;
  string str = ss.str();
  return str;
}

string Simulator::CovertFromDouble(double avg)
{
  ostringstream strs;
  strs << avg;
  string str = strs.str();
  return str;
}

vector<int> Simulator::GetAlgoResults(string algoName)
{
  
  //int size = 0;
  vector<int> results; 
  map< string, map<string, int> >::iterator it;
  it = _scoresMap.find(algoName);
  map<string, int> innerMap;
  if ( it == _scoresMap.end() )   { // not found
    //cout << "ERROR in Simulator::GetAlgoResults !!!" << endl;      // delete !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  else {  // found
    innerMap = (it -> second);
    map<string, int>::iterator innerIt;
    int res;
   
    for(innerIt = innerMap.begin() ; innerIt != innerMap.end(); innerIt++) {
      res = (innerIt -> second);
      results.push_back(res);
      
      //++size;
    }
  }
  
  //cout << algoName << ":  ";
  //cout << "size = " << size << endl; // delete !!!!!!!!!!!!!!!!!!!!!!!!!
  return results;
}

vector<string> Simulator::GetValidTrimmedHousesNames()
{
  vector<House *>::iterator houseIterator;
  string hName;   string hNameNoSuffix;  string trimmedHouseName;
  vector<string> trimmedHouseNames;
  for(houseIterator = _houses.begin() ; houseIterator != _houses.end(); houseIterator++) {
    hName = (*houseIterator)-> GetHouseFileName();
    //hNameNoSuffix = ...           //TODO:    get rid of the .house suffix !!!!!!!!!!!!!!
    //trimmedHouseName = hNameNoSuffix.substr(0,9);
    trimmedHouseName = hName.substr(0,9);
    trimmedHouseNames.push_back(trimmedHouseName);
  }
  return trimmedHouseNames;
}

void Simulator::printFirstRow(int houseNum, vector<string> trimmedValidHouseNames)
{
  //print left empty cell:
  cout << '|';
  printSpaces(13);
  vector<string>::iterator houseNamesIterator;
  string trimmedHouseName;
  for(houseNamesIterator = trimmedValidHouseNames.begin() ; houseNamesIterator != trimmedValidHouseNames.end(); houseNamesIterator++) {
    trimmedHouseName = (*houseNamesIterator);
    cout << '|';
    cout << trimmedHouseName;
    int hNameLength = trimmedHouseName.length();
    int quantity = 9- hNameLength + 1;
    printSpaces(quantity); 
  }
  cout << '|';
  cout << "AVG";
  printSpaces(7);
  cout << '|';
  cout << endl;
}

void Simulator::printDashes(int numCharsInRow)
{
  for(int i = 0; i < numCharsInRow; i++)
    cout << '-';
  cout << endl;
}

void Simulator::printSpaces(int quantity)
{
  for(int i = 0; i < quantity; i++)
    cout << " ";
}

void Simulator::addScore(string algoName, string trimmedHouseName, int score)
{
  
  // map<string, map<string, int> > _scoresMap     map<algo name, map<house name, int result> >
  map< string, map<string, int> >::iterator it;
  it = _scoresMap.find(algoName);
  map<string, int> innerMap;
  if ( it == _scoresMap.end() ) { // not found
    innerMap.insert( pair<string, int>(trimmedHouseName ,score) );
    _scoresMap.insert ( pair< string, map<string, int> >(algoName ,innerMap) );
   
  }else {  // found
  
    (it -> second).insert ( pair<string,int>(trimmedHouseName ,score) );
     _scoresMap[algoName] = (it -> second);
     
     //cout << "algo = " << algoName << endl;
     //cout << "house = " << trimmedHouseName << endl;
  }
  
}


void Simulator::PrintHouseErrors()
{
  map<string, string> m =  GetBadHousesMap();
  map<string, string>::iterator strIt;
  for(strIt = m.begin(); strIt != m.end(); strIt++) 
      cout << (strIt->first)<< ": " << (strIt->second) << endl;
}

void Simulator::PrintAlgoErrors()
{
  map<string,string> m = (AlgorithmFactory::getInstance().GetBadAlgoMap() );
  map<string, string>::iterator strIt;
  for(strIt = m.begin(); strIt != m.end(); strIt++) {
      string withSuffix = (strIt->first) + ".so";
      cout << withSuffix << ": " << (strIt->second) << endl;
      
  }

  int numHitWall = _hitWallErrorMessages.size();
  if(numHitWall == 0)
      return;
  
  for(vector<string>::const_iterator i = _hitWallErrorMessages.begin(); i != _hitWallErrorMessages.end(); ++i) {
    // process i
    cout << *i << endl; 
}
  
}


string* Simulator::GetAbsPath(string relativePath)
{
  
  char * relative = new char [relativePath.length()+1];
  std::strcpy (relative, relativePath.c_str());
  char actualpath [PATH_MAX + 1];
  char *ptr;
  ptr = realpath(relative, actualpath); 
  delete[] relative;
  std::string* abs = NULL;
  if(ptr != NULL){
    abs = new string(actualpath);  //calling function needs to delete abs in case it was allocateds
  }
  return abs;
  
//   std::string* mValue = NULL;
// 
// if (value) {
//     mValue = new std::string(value);
// }
}


//for debug
void Simulator::PrintDirection(Direction chosen)
{
	switch (chosen)
	{
	case Direction::North:
		cout << "North" << endl;
		return;
	case Direction::South:
		cout << "South" << endl;
		return;
	case Direction::East:
		cout << "East" << endl;
		return;
	case Direction::West:
		cout << "West" << endl;
		return;
	default:
		cout << "Stay" << endl;
	}
}
