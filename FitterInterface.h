// This file is part of the source code of Neurofitter 
// For Copyright(c) information see the file "AUTHORS"

#ifndef FITTERINTERFACE_H
#define FITTERINTERFACE_H

#include "FitnessCalculator.h"
#include "FitterResults.h"

using namespace std;

class FitterInterface {

public:
	//todo make it possible to give a list of startingpoints
	virtual FitterResults runFitter(ModelTuningParameters * startPoints) = 0;
	virtual ~FitterInterface() {};

protected:
	FitterInterface(FitnessCalculator* fit):fitness(fit) {};
	FitnessCalculator * fitness;
	ModelTuningParameters * modelParams;

private:
	FitterInterface();
};

#endif
