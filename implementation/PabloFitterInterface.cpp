#include "../PabloFitterInterface.h"

PabloFitterInterface::PabloFitterInterface(FitnessCalculator * fit, FixedParameters param) 
	: FitterInterface(fit), FixedParamObject(param) {}

FitterResults PabloFitterInterface::runFitter(ModelTuningParameters * startPoints, int seed) {
	///////////////////////
	/// Read parameters ///
	///////////////////////
	FixedParameters globalParam;
	globalParam["Dimensions"] = fixedParams["Dimensions"];
	globalParam["VerboseLevel"] = fixedParams["VerboseLevel"];
 
	FixedParameters eoFixParam = FixedParameters(fixedParams["EOFitter"], fixedParams.getGlobals());
	FixedParameters nomadFixParam = FixedParameters(fixedParams["NOMADFitter"], fixedParams.getGlobals());

	//////////////
	/// Run EO ///
	//////////////
	EOFitterInterface eo(fitness, eoFixParam);
	ModelTuningParameters eoBest = (eo.runFitter(startPoints, seed)).getBestFit();

	cout << endl << "Best solution found by EO: "<< eoBest.toString() << endl;

	//////////////////////////////////////////
	/// Run NOMAD with best solution of EO ///
	//////////////////////////////////////////
	NOMADFitterInterface nomad(fitness, nomadFixParam);
    nomad.runFitter(&eoBest, seed);

	return FitterResults();
}
