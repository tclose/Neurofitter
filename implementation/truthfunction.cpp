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
	//todo numberOfParameters is not well coded !!!
	params.setTuningParameters(paramValues, numberOfParameters);
	
	return fitness->calculateFitness(params);
}
