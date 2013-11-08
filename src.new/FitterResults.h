/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
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
