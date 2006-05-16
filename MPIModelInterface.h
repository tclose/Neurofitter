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
#include "MPIStream.h"

using namespace std;

class MPIModelInterface : public ModelInterface, public FixedParamObject {

public:	
	MPIModelInterface(FixedParameters params); 
	~MPIModelInterface();
    virtual ModelResults runModel(const ModelTuningParameters &);
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >); 

private:
	MPIModelInterface();
	int rank, ntasks;
	mpi_stream mpiChannel;
	ModelInterface * localModel;
	
	void runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params);
	void receiveResultsFromSlave(int & taskRank, vector< ModelResults > & results);
	void startSlave();
};

#endif

