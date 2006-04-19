#include "../PabloFitterInterface.h"

PabloFitterInterface::PabloFitterInterface(FitnessCalculator * fit) 
	: FitterInterface(fit) {}

FitterResults PabloFitterInterface::runFitter(ModelTuningParameters * startPoints, int seed) {
	EOFitterInterface eo(fitness);
	ModelTuningParameters eoBest = (eo.runFitter(startPoints, seed)).getBestFit();

	cout << endl << "Best solution found by EO: "<< eoBest.toString() << endl;

	NOMADFitterInterface nomad(fitness, startPoints->getLength());
    nomad.runFitter(&eoBest, seed);

	return FitterResults();
}
