#ifndef FITTERRESULTS_H
#define FITTERRESULTS_H

#include "ModelTuningParameters.h"

using namespace std;

///todo extend the members
class FitterResults {

	public:
		ModelTuningParameters getBestFit() const;		
		void setBestFit(const ModelTuningParameters);
		string getExtraResults() const;
		void setExtraResults(const string);

	protected:
		ModelTuningParameters bestFit;
		string extraResults;

};

#endif
