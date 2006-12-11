/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include <cmath>
#include <vector>
#include "nomad"
#include "../truthfunction.h"

using namespace std;

FitnessCalculator * fitness;
int numberOfParameters;

void setNOMADFitnessCalculator (FitnessCalculator * f, int newNumberOfParam){
	fitness = f;
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
	params.setTuningParameters(paramValues, numberOfParameters);
	
	fitness->calculateFitness(params);
	
	return params.getFitnessValue();
}

vector< double > TruthFunction::parallelEvaluate(const vector< double* > params) {

	vector< ModelTuningParameters > paramList(params.size());
	vector< double > paramValues(numberOfParameters); 

	for (int nParams = 0; nParams < (int)params.size(); nParams++) {
		for (int i = 0; i < numberOfParameters; i++) {
			paramValues[i] = params[nParams][i];
		}
		paramList[nParams].setTuningParameters(paramValues, numberOfParameters);
	}
	
	fitness->calculateParallelFitness(paramList);

	vector< double > fitnessValues(params.size());
	for (int i = 0; i < (int)params.size(); i++) {
		fitnessValues[i] = paramList[i].getFitnessValue();
	}

	return fitnessValues;

}

