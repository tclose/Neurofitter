#include "../WernerExperimentInterface.h"

WernerExperimentInterface::WernerExperimentInterface(FixedParameters params)
	: fixedParams(params) {}

ModelResults WernerExperimentInterface::getData() const{
	cout << atoi(fixedParams["Dimensions"].c_str());
	ModelTuningParameters params(fixedParams["Parameters"],atoi(fixedParams["Dimensions"].c_str()), NULL);	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(params);
}

