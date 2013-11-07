/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FitterResults.h"

FitterResults::FitterResults(vector< ModelTuningParameters > list) {
	bestErrorValue = list[0].getErrorValue();
	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].getErrorValue() <= bestErrorValue) {
			bestFit = list[i];
			bestErrorValue = list[i].getErrorValue();
		}
	}
}

ModelTuningParameters FitterResults::getBestFit() const {
	return bestFit;
}

double FitterResults::getBestErrorValue() const {
	return bestErrorValue;
}
		
void FitterResults::setBestFit(const ModelTuningParameters newBestFit, const double errorValue) {
	bestFit = newBestFit;
	bestErrorValue = errorValue;
}

/*
string FitterResults::getExtraResults() const{
	return extraResults;
}


void FitterResults::setExtraResults(const string newExtraResults) {
	extraResults = newExtraResults;
}*/
