#include <cmath>
#include "nomad"
#include "../truthfunction.h"

using namespace std;

FitnessCalculator * fitness;

void setNOMADFitnessCalculator (FitnessCalculator * f){
	fitness = f;
}

TruthFunction::TruthFunction() {

}

// Destructor.
TruthFunction::~TruthFunction() {

}

double TruthFunction::evaluate(const double * coord) {
	ModelTuningParameters params;
	//todo 4 is hard coded !!!
	params.setTuningParameters(coord, 4);
	
	return fitness->calculateFitness(params);
}
