#ifndef GENESISMODELINTERFACE_H
#define GENESISMODELINTERFACE_H

#include <vector> 
#include <fstream> 
using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"


class GenesisModelInterface : public ModelInterface, public FixedParamObject {

public:	
	GenesisModelInterface(FixedParameters params) : FixedParamObject(params) {}; 
    virtual ModelResults runModel(const ModelTuningParameters &) const;
	~GenesisModelInterface() {};

	void readDataFromFile(ModelResults&, string inputFileName, int startTrace, double injWeight) const;

private:

};

#endif
