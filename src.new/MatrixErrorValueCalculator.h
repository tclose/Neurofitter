/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
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
};

#endif
