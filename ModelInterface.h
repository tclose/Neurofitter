/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MODELINTERFACE_H
#define NEUROFITTER_MODELINTERFACE_H

#include "TracesReader.h"
#include "ModelResults.h"
#include "ModelTuningParameters.h"

using namespace std;

class ModelInterface {

public:
	ModelInterface(TracesReader * t) : tracesReader(t) {};

	virtual ModelResults runModel(const ModelTuningParameters &) = 0;
	virtual void runParallelModel(const vector< ModelTuningParameters >, vector< ModelResults> &) = 0;	
	virtual ~ModelInterface() {};

protected:	
	TracesReader * tracesReader;

};

#endif
