/*
Revision of last commit: $Rev: 168 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-06-21 11:23:19 +0900 (Thu, 21 Jun 2007) $
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

