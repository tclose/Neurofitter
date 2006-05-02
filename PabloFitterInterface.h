#ifndef PABLOFITTERINTERFACE_H
#define PABLOFITTERINTERFACE_H

#include "FitterInterface.h"
#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "FitnessCalculator.h"
#include "PabloFitnessCalculator.h"
#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "FixedParamObject.h"

///todo make all classes use initializers

class PabloFitterInterface : public FitterInterface, public FixedParamObject {

public:
	PabloFitterInterface(FitnessCalculator*, FixedParameters);
	////
	//Inherited from FitterInterface
	////
	FitterResults runFitter(ModelTuningParameters * startingPoint);

private:
	PabloFitterInterface();

};

#endif
