#include "../WernerExperimentInterface.h"

WernerExperimentInterface::WernerExperimentInterface(FixedParameters params)
	: FixedParamObject(params) {}

ModelResults WernerExperimentInterface::getData() const{
	ModelTuningParameters params(fixedParams["Parameters"],atoi(fixedParams["Dimensions"].c_str()), fixedParams["Bounds"]);	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(params);
}
