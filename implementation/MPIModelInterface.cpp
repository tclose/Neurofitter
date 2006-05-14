#include "../MPIModelInterface.h"

MPIModelInterface::MPIModelInterface(FixedParameters params) : 
	FixedParamObject(params) {

	MPI_Comm_rank( MPI_COMM_WORLD, &rank );

	if (fixedParams["ModelType"] == "Genesis") {
		localModel = new GenesisModelInterface(fixedParams["ModelParameters"]);
	}
	else crash("MPIModelInterface", "No matching model type");
	                
}

MPIModelInterface::~MPIModelInterface() {
	if (localModel != NULL) delete localModel;
}

ModelResults	MPIModelInterface::
runModel(const ModelTuningParameters & params) const {

	return localModel->runModel(params);

}

vector< ModelResults > MPIModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) const {

	return ;

}

