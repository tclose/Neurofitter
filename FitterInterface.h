#ifndef FITTERINTERFACE_H
#define FITTERINTERFACE_H

#include "FitnessCalculator.h"

class FitterInterface {

public:
	//todo make it possible to give a list of startingpoints
	virtual void runFitter(FitnessCalculator *, ModelTuningParameters *, int seed) = 0;
	virtual void readFixedParameters(const string);
	virtual ~FitterInterface() {};

protected:
	FitnessCalculator * fitness;
	ModelTuningParameters * modelParams;
};

#endif
