/*
Revision of last commit: $Rev: 275 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2008-07-01 10:18:46 +0900 (Tue, 01 Jul 2008) $
*/

#ifndef NEUROFITTER_RODRIGOERRORVALUECALCULATOR_H
#define NEUROFITTER_RODRIGOERRORVALUECALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ModelTuningParameters.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

class RodrigoErrorValueCalculator : public ErrorValueCalculator, public FixedParamObject {

public:
	RodrigoErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~RodrigoErrorValueCalculator();

	virtual void calculateErrorValue(ModelTuningParameters & param);
	virtual void calculateParallelErrorValue(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getErrorValueHistory();

private:
	double calculateRMSError(DataTrace &, DataTrace &);
	ofstream exportFileStream;
};

#endif
