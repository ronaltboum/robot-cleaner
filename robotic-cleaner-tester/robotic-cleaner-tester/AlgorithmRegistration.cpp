#include <iostream>
#include "AlgorithmFactory.h"
#include "Direction.h"
#include "AbstractSensor.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRegistration.h"

using namespace std;

//========================================================================
// AlgorithmRegistration.cpp is part of the simulation project ONLY
// You MUST implement ONE and ONLY ONE method, with this exact signature.
// But you have the freedom to implement it as you wish!
//========================================================================
AlgorithmRegistration::AlgorithmRegistration(std::function<unique_ptr<AbstractAlgorithm>()> algorithmFactoryFunc) {
    cout << "register!" << endl;
    //cout  << algorithmFactory;
    AlgorithmFactory::getInstance().registerAlgorithm(algorithmFactoryFunc);
}
