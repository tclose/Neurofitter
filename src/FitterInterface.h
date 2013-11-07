/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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
