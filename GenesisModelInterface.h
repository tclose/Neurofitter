#ifndef NEUROFITTER_GENESISMODELINTERFACE_H
#define NEUROFITTER_GENESISMODELINTERFACE_H

#include <vector> 
#include <fstream> 
using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class GenesisModelInterface : public ModelInterface, public FixedParamObject {

public:	
	GenesisModelInterface(TracesReader * t, FixedParameters params) : ModelInterface(t), FixedParamObject(params) {}; 
    virtual ModelResults runModel(const ModelTuningParameters &) ;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >);

private:

};

#endif
