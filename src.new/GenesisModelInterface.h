/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#ifndef NEUROFITTER_GENESISMODELINTERFACE_H
#define NEUROFITTER_GENESISMODELINTERFACE_H

#include <vector> 
#include <fstream> 
using namespace std;

#include "ModelInterface.h"
#include "ExecutableModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class GenesisModelInterface : public ModelInterface, public FixedParamObject {

public:	
	GenesisModelInterface(TracesReader * t, FixedParameters params);
    virtual ModelResults runModel(const ModelTuningParameters &) ;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >);

private:
	ExecutableModelInterface execModel;
	GenesisModelInterface();
};

#endif
