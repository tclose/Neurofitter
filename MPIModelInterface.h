#ifndef MPIMODELINTERFACE_H
#define MPIMODELINTERFACE_H

#include <vector> 
#include <fstream> 
#include <mpi.h> 

using namespace std;

#include "ModelInterface.h"
#include "FixedParamObject.h"

using namespace std;

class MPIModelInterface : public ModelInterface, public FixedParamObject {

public:	
	MPIModelInterface(FixedParameters params); 
    virtual ModelResults runModel(const ModelTuningParameters &) const;
	virtual vector< ModelResults > runParallelModel(const vector< ModelTuningParameters >) const; 

private:
	int rank;
	ModelInterface * localModel;
};

#endif

