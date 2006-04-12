#ifndef PABLOFITNESSCALCULATOR_H
#define PABLOFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"

class PabloFitnessCalculator : public FitnessCalculator {

public:
	~PabloFitnessCalculator();
	PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment); 

	virtual double calculateFitness(const ModelTuningParameters & param);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;


private:
	PabloVdVdtMatrix * expVdVdtMatrices;	
	PabloFitnessCalculator() {};
	ofstream exportFileStream;
};

#endif
