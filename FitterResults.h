#ifndef FITTERRESULTS_H
#define FITTERRESULTS_H

#include "ModelTuningParameters.h"

using namespace std;

///todo extend the members
class FitterResults {

	public:
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
