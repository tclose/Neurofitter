#ifndef FITTERINTERFACE_H
#define FITTERINTERFACE_H

#include "FitnessCalculator.h"
#include "FitterResults.h"

class FitterInterface {

public:
	//todo make it possible to give a list of startingpoints
	virtual FitterResults runFitter(ModelTuningParameters * startPoints, int seed) = 0;
//	virtual void readFixedParameters(const string);
	virtual ~FitterInterface() {};

protected:
	FitterInterface(FitnessCalculator* fit):fitness(fit) {};
	FitnessCalculator * fitness;
	ModelTuningParameters * modelParams;

private:
	FitterInterface();
};

#endif
