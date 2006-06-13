#ifndef PABLOFITNESSCALCULATOR_H
#define PABLOFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "NormalVdVdtMatrix.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

class MatrixFitnessCalculator : public FitnessCalculator, public FixedParamObject {

public:
	MatrixFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~MatrixFitnessCalculator();

	virtual double calculateFitness(ModelTuningParameters & param);
	virtual vector< double > calculateParallelFitness(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory();

private:
	vector< NormalVdVdtMatrix > expVdVdtMatrices;	
	ofstream exportFileStream;
};

#endif
