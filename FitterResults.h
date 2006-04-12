#include "ModelTuningParameters.h"

///todo extend the members
class FitterResults {

	public:
		ModelTuningParameters getBestFit() const;		
		void setBestFit(const ModelTuningParameters);
		string getExtraResults();
		void setExtraResults(string);

	protected:
		ModelTuningParameters bestFit;
		string extraResults;

};
