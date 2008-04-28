/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MATRIXERRORVALUECALCULATOR_H
#define NEUROFITTER_MATRIXERRORVALUECALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ModelTuningParameters.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"
#include "VdVdtMatrix.h"

class MatrixErrorValueCalculator : public ErrorValueCalculator, public FixedParamObject {

public:
	MatrixErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~MatrixErrorValueCalculator();

	virtual void calculateErrorValue(ModelTuningParameters & param);
	virtual void calculateParallelErrorValue(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getErrorValueHistory();

private:
	vector< VdVdtMatrix * > expVdVdtMatrices;	
	VdVdtMatrix * modelVdVdtMatrix;
	ofstream exportFileStream;
	
	bool printMatrix;
};

#endif
