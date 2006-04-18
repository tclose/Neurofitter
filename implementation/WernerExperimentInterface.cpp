#include "../WernerExperimentInterface.h"

WernerExperimentInterface::WernerExperimentInterface(ModelTuningParameters modelParams)
	: params(modelParams) {}

ModelResults WernerExperimentInterface::getData() const {
	const string          workpath = "/Users/werner/Desktop/EvolufitWork/wernermodel";
    const string          genpath = "/usr/local/bin/";
	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(params);
}

