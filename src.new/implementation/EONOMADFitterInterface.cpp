/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include "../EONOMADFitterInterface.h"

EONOMADFitterInterface::EONOMADFitterInterface(ErrorValueCalculator * fit, FixedParameters param) 
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
	EOFitterInterface eo(errorValue, eoFixParam);
	ModelTuningParameters eoBest = (eo.runFitter(startPoints)).getBestFit();

	showMessage("\nBest solution found by EO: "+ eoBest.toString() + "\n", 1, fixedParams);

	//////////////////////////////////////////
	/// Run NOMAD with best solution of EO ///
	//////////////////////////////////////////
	NOMADFitterInterface nomad(errorValue, nomadFixParam);
    nomad.runFitter(&eoBest);

	return FitterResults();
}
