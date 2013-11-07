/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FakeExperimentInterface.h"

FakeExperimentInterface::FakeExperimentInterface(ModelInterface * interface, FixedParameters params)
	: FixedParamObject(params), model(interface) {}

ModelResults FakeExperimentInterface::getData() const{
	ModelTuningParameters params(fixedParams["Parameters"],toInt(fixedParams["Dimensions"]), fixedParams["Bounds"]);	

	return model->runModel(params);
}
