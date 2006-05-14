#include "../MPIModelInterface.h"

const int tag = 42;
const int dietag = 42;

MPIModelInterface::MPIModelInterface(FixedParameters params) : 
	FixedParamObject(params) {

	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

	if (fixedParams["ModelType"] == "Genesis") {
		localModel = new GenesisModelInterface(fixedParams["ModelParameters"]);
	}
	else crash("MPIModelInterface", "No matching model type");
	                
	if (rank != 0) startSlave();
}

MPIModelInterface::~MPIModelInterface() {
	if (rank == 1) {
		for (int i = 1; i < ntasks; ++i) {
	    	MPI_Send(0, 0, MPI_INT, i, dietag, MPI_COMM_WORLD);
		}
	}
	if (localModel != NULL) delete localModel;
}

ModelResults	MPIModelInterface::
runModel(const ModelTuningParameters & params) const {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	return (runParallelModel(paramList))[0];

}

///todo run also a job on the master
vector< ModelResults > MPIModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) const {

	vector< ModelResults > results(paramList.size());
	
	int nSubmitted = 0;
	int nReceived = 0;
	int taskRank = 1;
	
	//Run models on all available slaves
	while (nSubmitted < (int)ntasks && nSubmitted < (int)paramList.size())
		runModelOnSlave(taskRank++, nSubmitted, paramList[nSubmitted++]);
	}

	//There are more jobs than slaves
	while (nSubmitted < paramList.size()) {
		receiveResultFromSlave(&taskRank, results);
		nReceived++;
		runModelOnSlave(taskRank, nSubmitted ,paramList[nSubmitted++]);
	}

	//Receive the remainder of the results
	while (nReceived < nSubmitted) {
		receiveResultFromSlave(&taskRank, results);
		nReceived++;
	}

	return results;

}

void MPIModelInterface::runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) {

	ostringstream paramStream;
	params.printOn(paramStream);
	string paramString = paramStream.str();
	unsigned int paramLength = paramString.length()+1 ;
	
	MPI_Send(&resultNumber,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD)
	MPI_Send(&paramLength,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD)
	MPI_Send(paramString.c_str(),paramString.length(),MPI_CHAR,slaveNumber,tag,MPI_COMM_WORLD);

}

void MPIModelInterface::receiveResultsFromSlave(int * taskRank, vector< ModelResults > & results) {

	int resultNumber;
	int resultLength;
	MPI_Status staus;
	
	MPI_Recv(&resultNumber,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	MPI_Recv(&resultLength,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	
	char * resultString = new char[resultLength];
	MPI_Recv(resultString,resultLength,MPI_CHAR,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	istringstream stringStream(string(resultString),istringstream::in);
	
	results[resultNumber].readFrom(resultStream);
	
	delete resultString;
}

void MPIModelInterface::startSlave() {

	int resultNumber;
	int parametersLength;
	int resultsLength;
	int err;
	MPI_Status status;
	
	while (true) {
		MPI_Recv(&resultNumber, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
		if (status.MPI_TAG == DIETAG) return;

		MPI_Recv(&parametersLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 

	
		char * parametersString = new char[parametersLength];
		MPI_Recv(parametersString, parametersLength, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

		istringstream parametersStream(string(parametersString),istringstream::in);

		ModelTuningParameters parameters;
		parameters.readFrom(parametersStream);
	
		ModelResults result = localModel->runModel(parameters);
		
		ostringstream resultStream;
		result.printOn(resultStream);
		string resultString = resultStream.str();
		resultLength = resultString.length()+1;

		MPI_Send(&resultNumber, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(&resultLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(resultString.c_str(), resultLength, MPI_INT, 0, tag, MPI_COMM_WORLD);
	
		delete parametersString;
	}
}


