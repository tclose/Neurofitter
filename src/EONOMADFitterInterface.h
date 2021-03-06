/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EONOMADFITTERINTERFACE_H
#define NEUROFITTER_EONOMADFITTERINTERFACE_H

#include "FitterInterface.h"
#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ErrorValueCalculator.h"
#include "MatrixErrorValueCalculator.h"
#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "FixedParamObject.h"

using namespace std;

class EONOMADFitterInterface : public FitterInterface, public FixedParamObject {

public:
	EONOMADFitterInterface(ErrorValueCalculator*, FixedParameters);
	////
	//Inherited from FitterInterface
	////
	FitterResults runFitter(ModelTuningParameters * startingPoint);

private:
	EONOMADFitterInterface();

};

#endif
