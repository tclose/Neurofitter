#include "../WernerExperimentInterface.h"

ModelResults WernerExperimentInterface::getData() const {
	const string          workpath = "/Users/werner/Desktop/EvolufitWork/wernermodel";
    const string          genpath = "/usr/local/bin/";

	double dataparams [] = {2800.0, 11200.0};
	
	WernerModelInterface interface = WernerModelInterface();

	return interface.runModel(ModelTuningParameters(dataparams,2, NULL));
}

