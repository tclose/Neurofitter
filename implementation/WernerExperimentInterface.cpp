#include "../WernerExperimentInterface.h"

ModelResults WernerExperimentInterface::getData() const {
	const string          workpath = "/Users/werner/Desktop/EvolufitWork/wernermodel";
    const string          genpath = "/usr/local/bin/";

	double dataparams [] = {10.0, 6000.0};
	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(ModelTuningParameters(dataparams,2, NULL));
}

