/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MPIMODELINTERFACE_H
#define NEUROFITTER_MPIMODELINTERFACE_H

#include <mpi.h> 

#include <vector> 
#include <sstream> 
#include <fstream> 

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
	MPIModelInterface(TracesReader*, FixedParameters params); 
	~MPIModelInterface();
    virtual ModelResults runModel(const ModelTuningParameters &);
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >); 

private:
	MPIModelInterface();
	int rank, ntasks;
	MPIStream mpiChannel;
	ModelInterface * localModel;
	
	void runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params);
	void receiveResultsFromSlave(int & taskRank, vector< ModelResults > & results);
	void startSlave();
};

#endif

