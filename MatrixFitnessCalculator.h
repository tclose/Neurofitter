#ifndef NEUROFITTER_MATRIXFITNESSCALCULATOR_H
#define NEUROFITTER_MATRIXFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"
#include "VdVdtMatrix.h"

class MatrixFitnessCalculator : public FitnessCalculator, public FixedParamObject {

public:
	MatrixFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~MatrixFitnessCalculator();

	virtual void calculateFitness(ModelTuningParameters & param);
	virtual void calculateParallelFitness(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory();

private:
	vector< VdVdtMatrix * > expVdVdtMatrices;	
	VdVdtMatrix * modelVdVdtMatrix;
	ofstream exportFileStream;
};

#endif
