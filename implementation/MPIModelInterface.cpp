
#include "../MPIModelInterface.h"

const int tag = 42;
const int dietag = 666;

MPIModelInterface::MPIModelInterface(FixedParameters params) : 
	FixedParamObject(params) {

	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

	if (fixedParams["ModelType"] == "Genesis") {
		localModel = new GenesisModelInterface(FixedParameters(fixedParams["ModelParameters"],fixedParams.getGlobals()));
	}
	else crash("MPIModelInterface", "No matching model type");
	                
	if (rank != 0) startSlave();
}

MPIModelInterface::~MPIModelInterface() {
	if (rank == 1) {
		for (int i = 1; i < ntasks; ++i) {
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending kill signal to slaves" << endl;
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
	while (nSubmitted < (int)ntasks && nSubmitted < (int)paramList.size()) {
		runModelOnSlave(taskRank++, nSubmitted, paramList[nSubmitted++]);
	}

	//There are more jobs than slaves
	while (nSubmitted < (int)paramList.size()) {
		receiveResultsFromSlave(&taskRank, results);
		nReceived++;
		runModelOnSlave(taskRank, nSubmitted ,paramList[nSubmitted++]);
	}

	//Receive the remainder of the results
	while (nReceived < nSubmitted) {
		receiveResultsFromSlave(&taskRank, results);
		nReceived++;
	}

	return results;

}

void MPIModelInterface::runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) const {

	ostringstream paramStream;
	params.printOn(paramStream);
	string paramString = paramStream.str();
	unsigned int paramLength = paramString.length()+1 ;

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending parameters to slave: " << slaveNumber << "... ";
	MPI_Send(&resultNumber,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD);
	MPI_Send(&paramLength,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD);
	MPI_Send((void*)paramString.c_str(),paramString.length(),MPI_CHAR,slaveNumber,tag,MPI_COMM_WORLD);
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Parameters sent " << endl;

}

void MPIModelInterface::receiveResultsFromSlave(int * taskRank, vector< ModelResults > & results) const {

	int resultNumber;
	int resultLength;
	MPI_Status status;

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Waiting for  results from slave ... ";	
	MPI_Recv(&resultNumber,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	MPI_Recv(&resultLength,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	
	char * resultString = new char[resultLength];
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Receiving results from slave ... ";	
	MPI_Recv(resultString,resultLength,MPI_CHAR,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Results received results" << endl;

	cout << string(resultString) << endl;

	istringstream resultStream; 
	resultStream.str(string(resultString));
	
	results[resultNumber].readFrom(resultStream);
	
	delete resultString;
}

void MPIModelInterface::startSlave() {

	int resultNumber;
	int parametersLength;
	int resultsLength;
	MPI_Status status;
	
	while (true) {
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " is waiting for parameters from master ..." << endl;
		MPI_Recv(&resultNumber, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 
		if (status.MPI_TAG == dietag) return;

		MPI_Recv(&parametersLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 

		char * parametersString = new char[parametersLength];
		MPI_Recv(parametersString, parametersLength, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

		if (toInt(fixedParams["VerboseLevel"]) > 4) cout << "Slave " << rank << " received parameters string from master: " << string(parametersString) <<  endl;

		istringstream parametersStream;
		parametersStream.str(string(parametersString));

		ModelTuningParameters parameters;
		parameters.readFrom(parametersStream);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " running model with parameters: " << parameters.toString() << endl;
	
		ModelResults result = localModel->runModel(parameters);
		
		ostringstream resultStream;
		result.printOn(resultStream);
		string resultString = resultStream.str();
		resultsLength = resultString.length()+1;

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " sending results back to master ... ";

		MPI_Send(&resultNumber, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send(&resultsLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		MPI_Send((void*)resultString.c_str(), resultsLength, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has sent results back" << endl;
	
		delete parametersString;
	}
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has stopped" << endl;
}


