#include "../FitterResults.h"

ModelTuningParameters FitterResults::getBestFit() const {
	return bestFit;
}

double FitterResults::getBestFitness() const {
	return bestFitness;
}
		
void FitterResults::setBestFit(const ModelTuningParameters newBestFit, const double fitness) {
	bestFit = newBestFit;
	bestFitness = fitness;
}

string FitterResults::getExtraResults() const{
	return extraResults;
}


void FitterResults::setExtraResults(const string newExtraResults) {
	extraResults = newExtraResults;
}
