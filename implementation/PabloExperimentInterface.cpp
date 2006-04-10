#include "../PabloExperimentInterface.h"

ModelResults PabloExperimentInterface::getData() {
	const string          workpath = "/Users/werner/Desktop/EvolufitWork/pablomodel";
    const string          genpath = "/usr/local/bin/";

    const int       numberOfInjections = 1;
    const int       numberOfRecordSites = 3;
    const int       numberOfPeriods = 2;

	ModelResults    results(numberOfInjections*numberOfRecordSites*numberOfPeriods);

	string inputname = workpath + "/results/PurkM9_soma_" + str(0) + "nA_data5";
	PabloModelInterface::readDataFromFile(results,inputname,0,3,2); 
	return results;
}






	/*
	ModelTuningParameters modelparams = ModelTuningParameters();
    double testSet [] = {75000.0,10.0,5.00,5.00,5.00,5.00,
						45.0,45.0,45.0,150.0,20.0,6000.0,
						600.0,0.400,0.100,0.130,0.130,800.0,
						800.0,800.0,3.90,3.90,3.90,3.0};

    modelparams.setTuningParameters(testSet,24);

	PabloModelInterface model = PabloModelInterface();
	return model.runModel(modelparams);
	*/


