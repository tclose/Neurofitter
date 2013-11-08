/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#include "../FakeExperimentInterface.h"

FakeExperimentInterface::FakeExperimentInterface(ModelInterface * interface, FixedParameters params)
	: FixedParamObject(params), model(interface) {}

ModelResults FakeExperimentInterface::getData() const{
	ModelTuningParameters params(fixedParams["Parameters"],toInt(fixedParams["Dimensions"]), fixedParams["Bounds"]);	

	return model->runModel(params);
}
