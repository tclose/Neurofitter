#ifndef MPIMODELINTERFACE_H
#define MPIMODELINTERFACE_H

#include <vector> 
#include <sstream> 
#include <fstream> 
#include <mpi.h> 

using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"
#include "GenesisModelInterface.h"
#include "ModelResults.h"
#include "ModelTuningParameters.h"

using namespace std;

class MPIModelInterface : public ModelInterface, public FixedParamObject {

public:	
	MPIModelInterface(FixedParameters params); 
	~MPIModelInterface();
    virtual ModelResults runModel(const ModelTuningParameters &) const;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >) const; 

private:
	MPIModelInterface();
	int rank, ntasks;
	ModelInterface * localModel;
	
	void runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) const;
	void receiveResultsFromSlave(int * taskRank, vector< ModelResults > & results) const;
	void startSlave();
};

#endif

