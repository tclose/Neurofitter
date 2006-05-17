
#include "../MPIModelInterface.h"

const int tag = 42;
const int dietag = 666;

MPIModelInterface::MPIModelInterface(FixedParameters params) : 
	FixedParamObject(params), mpiChannel(MPI_COMM_WORLD) {

	mpiChannel.setMessageId(tag);
	rank = mpiChannel.getRank();
	ntasks = mpiChannel.getSize();
	
	if (fixedParams["ModelType"] == "Genesis") {
		localModel = new GenesisModelInterface(FixedParameters(fixedParams["ModelParameters"],fixedParams.getGlobals()));
	}
	else crash("MPIModelInterface", "No matching model type");
	                
	if (rank != 0) startSlave();
}

MPIModelInterface::~MPIModelInterface() {
	if (rank == 0) {
		for (int i = 1; i < ntasks; ++i) {
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending kill signal to slave " << i << endl;
			mpiChannel.setMessageId(dietag);
			mpiChannel.setMessageRank(i);
			mpiChannel << 0;
		}
	}
	if (localModel != NULL) delete localModel;
}

ModelResults	MPIModelInterface::
runModel(const ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	return (runParallelModel(paramList))[0];

}

///todo run also a job on the master
vector< ModelResults > MPIModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	vector< ModelResults > results(paramList.size());

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Running "<< paramList.size() << " jobs in parallel" << endl;
	
	int nSubmitted = 0;
	int nReceived = 0;
	int taskRank = 1;
	
	//Run models on all available slaves
	while (nSubmitted < (int)ntasks-1 && nSubmitted < (int)paramList.size()) {
		runModelOnSlave(taskRank++, nSubmitted, paramList[nSubmitted]);
		nSubmitted++;
	}
	

	//There are more jobs than slaves
	while (nSubmitted < (int)paramList.size()) {
		receiveResultsFromSlave(taskRank, results);
		nReceived++;
		runModelOnSlave(taskRank, nSubmitted ,paramList[nSubmitted]);
		nSubmitted++;
	}

	//Receive the remainder of the results
	while (nReceived < nSubmitted) {
		receiveResultsFromSlave(taskRank, results);
		nReceived++;
	}

	return results;

}

void MPIModelInterface::runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) {

	//ostringstream paramStream;
	//string paramString = paramStream.str();
	//unsigned int paramLength = paramString.length()+1 ;

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending parameters to slave: " << slaveNumber << "... ";
	mpiChannel.setMessageRank(slaveNumber);
	mpiChannel << resultNumber;
	params.printOn(mpiChannel);
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Parameters sent " << endl;
	
}

void MPIModelInterface::receiveResultsFromSlave(int & taskRank, vector< ModelResults > & results) {

	int resultNumber;
	//int resultLength;
	//MPI_Status status;

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Waiting for  results from slave ... ";	
	mpiChannel.setMessageRank(MPI_ANY_SOURCE);
	mpiChannel >> resultNumber;
	taskRank = mpiChannel.getMessageRank();

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Receiving result " << resultNumber <<" from slave " << taskRank << "... ";	
	
	results[resultNumber].readFrom(mpiChannel);

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Results received results" << endl;
	
}

void MPIModelInterface::startSlave() {

	int resultNumber;
	MPI_Status status;
	
	while (true) {
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " is waiting for parameters from master ..." << endl;
		mpiChannel.setMessageRank(0);
		mpiChannel.setMessageId(MPI_ANY_TAG);
		mpiChannel >> resultNumber;
		if (mpiChannel.getMessageId() == dietag) {
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " received a kill signal from master" << endl;
			return;
		}
		else if (mpiChannel.getMessageId() != tag) {
			crash("MPIModelInterface", "Slave "+str(rank)+" received message with wrong tag: "+str(status.MPI_TAG));
		}
		mpiChannel.setMessageId(tag);

		ModelTuningParameters parameters;
		parameters.readFrom(mpiChannel);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " running model with parameters: " << parameters.toString() << endl;

		ModelResults result = localModel->runModel(parameters);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " sending results back to master ... ";

		mpiChannel.setMessageRank(0);
		mpiChannel << resultNumber;
		result.printOn(mpiChannel);
		
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has sent results back" << endl;

	}
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has stopped" << endl;
}


