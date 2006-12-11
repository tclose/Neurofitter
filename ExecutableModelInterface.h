/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EXECUTABLEMODELINTERFACE_H
#define NEUROFITTER_EXECUTABLEMODELINTERFACE_H

#include <vector> 
#include <fstream> 

#include "ModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class ExecutableModelInterface : public ModelInterface, public FixedParamObject {

public:	
	ExecutableModelInterface(TracesReader * t, FixedParameters params) : ModelInterface(t), FixedParamObject(params) {}; 
    virtual ModelResults runModel(const ModelTuningParameters &) ;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >);

private:

};

#endif
