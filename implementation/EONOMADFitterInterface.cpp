#include "../EONOMADFitterInterface.h"

EONOMADFitterInterface::EONOMADFitterInterface(FitnessCalculator * fit, FixedParameters param) 
	: FitterInterface(fit), FixedParamObject(param) {}

FitterResults EONOMADFitterInterface::runFitter(ModelTuningParameters * startPoints) {
	///////////////////////
	/// Read parameters ///
	///////////////////////
 
	FixedParameters eoFixParam = FixedParameters(fixedParams["EOFitterParameters"], fixedParams.getGlobals());
	FixedParameters nomadFixParam = FixedParameters(fixedParams["NOMADFitterParameters"], fixedParams.getGlobals());

	//////////////
	/// Run EO ///
	//////////////
	EOFitterInterface eo(fitness, eoFixParam);
	ModelTuningParameters eoBest = (eo.runFitter(startPoints)).getBestFit();

	showMessage("\nBest solution found by EO: "+ eoBest.toString() + "\n", 1, fixedParams);

	//////////////////////////////////////////
	/// Run NOMAD with best solution of EO ///
	//////////////////////////////////////////
	NOMADFitterInterface nomad(fitness, nomadFixParam);
    nomad.runFitter(&eoBest);

	return FitterResults();
}
