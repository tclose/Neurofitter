/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/


#include "../MPIModelInterface.h"

const int tag = 42;
const int dietag = 666; //Qui habet intellectum, computet numerum bestiae

MPIModelInterface::MPIModelInterface(TracesReader * t, FixedParameters params) : 
	ModelInterface(t), FixedParamObject(params), mpiChannel(MPI_COMM_WORLD) {

	mpiChannel.setMessageId(tag);
	rank = mpiChannel.getRank();
	ntasks = mpiChannel.getSize();
	
	if (fixedParams["ModelType"] == "Genesis") {
		localModel = new GenesisModelInterface(tracesReader, FixedParameters(fixedParams["ModelParameters"],fixedParams.getGlobals()));
	}
	else crash("MPIModelInterface", "No matching model type");
	                
	if (rank != 0) startSlave();
}

MPIModelInterface::~MPIModelInterface() {
	if (rank == 0) {
		for (int i = 1; i < ntasks; ++i) {
			showMessage("Sending kill signal to slave " + str(i) + "\n",4,fixedParams);
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
	vector< ModelResults > result(1);
	paramList[0] = params;
	runParallelModel(paramList, result);
	return result[0];

}

void MPIModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList, vector< ModelResults > & results) {

	showMessage("Running " + str((int)paramList.size()) + " jobs in parallel\n",4,fixedParams);
	
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

}

void MPIModelInterface::runModelOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) {

	//ostringstream paramStream;
	//string paramString = paramStream.str();
	//unsigned int paramLength = paramString.length()+1 ;

	showMessage("Sending parameters to slave: " + str(slaveNumber) + "... ",4,fixedParams);

	mpiChannel.setMessageRank(slaveNumber);
	mpiChannel << resultNumber;
	params.printOn(mpiChannel);

	showMessage(" Parameters sent \n",4,fixedParams);
	
}

void MPIModelInterface::receiveResultsFromSlave(int & taskRank, vector< ModelResults > & results) {

	int resultNumber;

	showMessage("Waiting for  results from slave ... ",4,fixedParams);

	mpiChannel.setMessageRank(MPI_ANY_SOURCE);
	mpiChannel >> resultNumber;
	taskRank = mpiChannel.getMessageRank();

	showMessage("Receiving result " + str(resultNumber) + " from slave " + str(taskRank) + "... ",4,fixedParams);
	
	results[resultNumber].readFrom(mpiChannel);

	showMessage(" Results received results\n",4,fixedParams);
	
}

void MPIModelInterface::startSlave() {

	int resultNumber;
	MPI_Status status;
	
	while (true) {
		showMessage("Slave " + str(rank) +  " is waiting for parameters from master ...\n",4,fixedParams);
		mpiChannel.setMessageRank(0);
		mpiChannel.setMessageId(MPI_ANY_TAG);
		mpiChannel >> resultNumber;
		if (mpiChannel.getMessageId() == dietag) {
			showMessage("Slave " + str(rank) +  " received a kill signal from master\n",4,fixedParams);
			return;
		}
		else if (mpiChannel.getMessageId() != tag) {
			crash("MPIModelInterface", "Slave "+str(rank)+" received message with wrong tag: "+str(status.MPI_TAG));
		}
		mpiChannel.setMessageId(tag);

		ModelTuningParameters parameters;
		parameters.readFrom(mpiChannel);

		showMessage("Slave " + str(rank) + " running model with parameters: " + parameters.toString() + "\n",4,fixedParams);

		ModelResults result = localModel->runModel(parameters);

		showMessage("Slave " + str(rank) + " sending results back to master ... ",4,fixedParams);

		mpiChannel.setMessageRank(0);
		mpiChannel << resultNumber;
		result.printOn(mpiChannel);

		showMessage("Slave " + str(rank) + " has sent results back\n",4,fixedParams);
	}
	showMessage("Slave " + str(rank) + " has stopped\n",4,fixedParams);
}


