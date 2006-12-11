/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FitterResults.h"

FitterResults::FitterResults(vector< ModelTuningParameters > list) {
	bestFitness = list[0].getFitnessValue();
	for (int i = 0; i < (int)list.size(); i++) {
		if (list[i].getFitnessValue() <= bestFitness) {
			bestFit = list[i];
			bestFitness = list[i].getFitnessValue();
		}
	}
}

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

/*
string FitterResults::getExtraResults() const{
	return extraResults;
}


void FitterResults::setExtraResults(const string newExtraResults) {
	extraResults = newExtraResults;
}*/
