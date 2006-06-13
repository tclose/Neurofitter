#ifndef PABLOFITTERINTERFACE_H
#define PABLOFITTERINTERFACE_H

#include "FitterInterface.h"
#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "FitnessCalculator.h"
#include "MatrixFitnessCalculator.h"
#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "FixedParamObject.h"

using namespace std;

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
