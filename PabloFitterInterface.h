#ifndef PABLOFITTERINTERFACE_H
#define PABLOFITTERINTERFACE_H

#include "FitterInterface.h"
#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "FitnessCalculator.h"
#include "PabloFitnessCalculator.h"
#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"

///todo make all classes use initializers

class PabloFitterInterface : FitterInterface {

public:
	PabloFitterInterface(ModelInterface*, ExperimentInterface*);
	~PabloFitterInterface();
	////
	//Inherited from FitterInterface
	////
	FitterResults runFitter(ModelTuningParameters * startingPoint, int seed);

private:
	PabloFitterInterface();

};

#endif
