/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

// This file is part of the source code of Neurofitter 
// For Copyright(c) information see the file "AUTHORS"

#ifndef NEUROFITTER_FITTERINTERFACE_H
#define NEUROFITTER_FITTERINTERFACE_H

#include "ErrorValueCalculator.h"
#include "FitterResults.h"

using namespace std;

class FitterInterface {

public:
	virtual FitterResults runFitter(ModelTuningParameters * startPoints) = 0;
	virtual ~FitterInterface() {};

protected:
	FitterInterface(ErrorValueCalculator* fit):errorValue(fit) {};
	ErrorValueCalculator * errorValue;
	ModelTuningParameters * modelParams;

private:
	FitterInterface();
};

#endif
