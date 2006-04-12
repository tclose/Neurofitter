#include "../FitterResults.h"

ModelTuningParameters FitterResults::getBestFit() const {
	return bestFit;
}
		
void FitterResults::setBestFit(const ModelTuningParameters newBestFit) {
	bestFit = newBestFit;
}

string FitterResults::getExtraResults() const{
	return extraResults;
}


void FitterResults::setExtraResults(const string newExtraResults) {
	extraResults = newExtraResults;
}
