/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

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
		double getBestErrorValue() const;
		void setBestFit(const ModelTuningParameters, const double errorValue);
		//string getExtraResults() const;
		//void setExtraResults(const string);

	protected:
		ModelTuningParameters bestFit;
		double bestErrorValue;
		//string extraResults;

};

#endif
