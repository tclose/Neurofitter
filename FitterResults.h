#ifndef NEUROFITTER_FITTERRESULTS_H
#define NEUROFITTER_FITTERRESULTS_H

#include <vector>

#include "ModelTuningParameters.h"

using namespace std;

class FitterResults {

	public:
		FitterResults() {};
		FitterResults(vector< ModelTuningParameters >);
		ModelTuningParameters getBestFit() const;
		double getBestFitness() const;
		void setBestFit(const ModelTuningParameters, const double fitness);
		//string getExtraResults() const;
		//void setExtraResults(const string);

	protected:
		ModelTuningParameters bestFit;
		double bestFitness;
		//string extraResults;

};

#endif
