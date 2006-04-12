#include "../PabloFitterInterface.h"

PabloFitterInterface::PabloFitterInterface(ModelInterface * model, ExperimentInterface * experiment) : FitterInterface(new PabloFitnessCalculator(model, experiment)) {}

PabloFitterInterface::~PabloFitterInterface() {
	if (fitness != NULL) delete fitness;
}

FitterResults PabloFitterInterface::runFitter(ModelTuningParameters * startPoints, int seed) {
	EOFitterInterface eo(fitness);
	ModelTuningParameters eoBest = (eo.runFitter(startPoints, seed)).getBestFit();

	cout << eoBest.toString();

	/*NOMADFitterInterface nomad(fitness);
    nomad.runFitter(&eoBest, seed);
*/
	return FitterResults();
}
