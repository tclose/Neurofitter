#ifndef FITNESSCALCULATOR_H
#define FITNESSCALCULATOR_H

#include "ModelTuningParameters.h"
#include "ModelInterface.h"

class FitnessCalculator {

public:
	virtual double calculateFitness(const ModelTuningParameters & params) const = 0;
	virtual void readFixedParameters(const string);
	virtual ~FitnessCalculator() {};

protected:
	const ModelInterface * model;
	//const ModelResults expData;
};

#endif
