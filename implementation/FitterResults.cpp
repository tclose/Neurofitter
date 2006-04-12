#include "../FitterResults.h"

ModelTuningParameters FitterResults::getBestFit() const {
	return bestFit;
}
		
void FitterResults::setBestFit(const ModelTuningParameters newBestFit) {
	bestFit = newBestFit;
}

string FitterResults::getExtraResults() {
	return extraResults;
}


void FitterResults::setExtraResults(string newExtraResults) {
	extraResults = newExtraResults;
}
