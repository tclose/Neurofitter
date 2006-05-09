#ifndef GENESISMODELINTERFACE_H
#define GENESISMODELINTERFACE_H

#include <vector> 
#include <fstream> 
using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class GenesisModelInterface : public ModelInterface, public FixedParamObject {

public:	
	GenesisModelInterface(FixedParameters params) : FixedParamObject(params) {}; 
    virtual ModelResults runModel(const ModelTuningParameters &) const;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >) const; //{crash("GenesisModel","No parallelization implemented");return (vector< ModelResults >());};

	void readDataFromFile(ModelResults&, string inputFileName, int startTrace, double injWeight) const;

private:

};

#endif
