#ifndef PABLOFITNESSCALCULATOR_H
#define PABLOFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

class PabloFitnessCalculator : public FitnessCalculator, public FixedParamObject {

public:
	PabloFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~PabloFitnessCalculator();

	virtual double calculateFitness(ModelTuningParameters & param);
	virtual vector< double > calculateParallelFitness(vector< ModelTuningParameters > params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory();

private:
	vector< PabloVdVdtMatrix > expVdVdtMatrices;	
	ofstream exportFileStream;
};

#endif
