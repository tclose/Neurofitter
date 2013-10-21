/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include <cmath>
#include <vector>
#include "nomad"
#include "../truthfunction.h"

using namespace std;

ErrorValueCalculator * errorValue;
int numberOfParameters;

void setNOMADErrorValueCalculator (ErrorValueCalculator * f, int newNumberOfParam){
	errorValue = f;
	numberOfParameters = newNumberOfParam;
}

TruthFunction::TruthFunction() {

}

// Destructor.
TruthFunction::~TruthFunction() {

}

double TruthFunction::evaluate(const double * coord) {
	ModelTuningParameters params;

	vector< double > paramValues(numberOfParameters);
	for (int i = 0; i < numberOfParameters; i++) {
		paramValues[i] = coord[i];
	}
	params.setTuningParameters(paramValues);
	
	errorValue->calculateErrorValue(params);
	
	return params.getErrorValue();
}

vector< double > TruthFunction::parallelEvaluate(const vector< double* > params) {

	vector< ModelTuningParameters > paramList(params.size());
	vector< double > paramValues(numberOfParameters); 

	for (int nParams = 0; nParams < (int)params.size(); nParams++) {
		for (int i = 0; i < numberOfParameters; i++) {
			paramValues[i] = params[nParams][i];
		}
		paramList[nParams].setTuningParameters(paramValues);
	}
	
	errorValue->calculateParallelErrorValue(paramList);

	vector< double > errorValues(params.size());
	for (int i = 0; i < (int)params.size(); i++) {
		errorValues[i] = paramList[i].getErrorValue();
	}

	return errorValues;

}

