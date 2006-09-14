#ifndef NEUROFITTER_NEURONMODELINTERFACE_H
#define NEUROFITTER_NEURONMODELINTERFACE_H

#include <vector> 
#include <fstream> 
using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class NeuronModelInterface : public ModelInterface, public FixedParamObject {

public:	
	NeuronModelInterface(TracesReader * t, FixedParameters params) : ModelInterface(t), FixedParamObject(params) {}; 
    virtual ModelResults runModel(const ModelTuningParameters &) ;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >);

private:

};

#endif
