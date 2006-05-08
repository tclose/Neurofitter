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
	//todo numberOfParameters is not well coded (static) !!!
	params.setTuningParameters(paramValues, numberOfParameters);
	
	return fitness->calculateFitness(params);
}

vector< double > TruthFunction::parallelEvaluate(const vector< double* > params) {

	vector < double > fitnessValues(params.size());	

	for (unsigned int i = 0; i < params.size(); i++) {
		fitnessValues[i] = this->evaluate(params[i]);
	}

	return fitnessValues;

}

