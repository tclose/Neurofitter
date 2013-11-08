/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
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
