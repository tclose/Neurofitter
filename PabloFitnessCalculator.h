#ifndef PABLOFITNESSCALCULATOR_H
#define PABLOFITNESSCALCULATOR_H

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"

class PabloFitnessCalculator : public FitnessCalculator {

public:
	PabloFitnessCalculator() {};
	~PabloFitnessCalculator();
	PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment); 

	virtual double calculateFitness(const ModelTuningParameters & param) const;

private:
	PabloVdVdtMatrix * expVdVdtMatrices;	

};

#endif
