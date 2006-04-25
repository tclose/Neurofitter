#include "../WernerExperimentInterface.h"

WernerExperimentInterface::WernerExperimentInterface(ModelInterface * interface, FixedParameters params)
	: FixedParamObject(params), model(interface) {}

ModelResults WernerExperimentInterface::getData() const{
	ModelTuningParameters params(fixedParams["Parameters"],toInt(fixedParams["Dimensions"]), fixedParams["Bounds"]);	

	return model->runModel(params);
}
