#ifndef PABLOFITNESSCALCULATOR_H
#define PABLOFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

class PabloFitnessCalculator : public FitnessCalculator, public FixedParamObject {

public:
	PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment, const FixedParameters); 
	~PabloFitnessCalculator();

	virtual double calculateFitness(const ModelTuningParameters & param);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;


private:
	PabloVdVdtMatrix * expVdVdtMatrices;	
	ofstream exportFileStream;
};

#endif
