/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
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
