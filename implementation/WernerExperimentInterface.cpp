#include "../WernerExperimentInterface.h"

WernerExperimentInterface::WernerExperimentInterface(FixedParameters params)
	: FixedParamObject(params) {}

ModelResults WernerExperimentInterface::getData() const{
	ModelTuningParameters params(fixedParams["Parameters"],toInt(fixedParams["Dimensions"]), fixedParams["Bounds"]);	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(params);
}
