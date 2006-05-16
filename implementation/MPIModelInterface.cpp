
#include "../MPIModelInterface.h"

const int tag = 42;
const int dietag = 666;

MPIModelInterface::MPIModelInterface(FixedParameters params) : 
	FixedParamObject(params), mpiChannel(COMM_WORLD) {

	//MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	//MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
	
	//mpiChannel = new mpi_stream(COMM_WORLD);
	mpiChannel.messageId(tag);
	rank = mpiChannel.rank();
	ntasks = mpiChannel.size();
	
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
			mpiChannel.messageId(dietag);
			mpiChannel.messageRank(i);
	    	//MPI_Send(0, 0, MPI_INT, i, dietag, MPI_COMM_WORLD);
		}
	}
	if (localModel != NULL) delete localModel;
	//if (mpiChannel != NULL) delete mpiChannel;
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
	mpiChannel.messageRank(slaveNumber);
	mpiChannel << resultNumber;
	params.printOn(mpiChannel);
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Parameters sent " << endl;
	
	//MPI_Send(&resultNumber,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD);
	//MPI_Send(&paramLength,1,MPI_INT,slaveNumber,tag,MPI_COMM_WORLD);
	//MPI_Send((void*)paramString.c_str(),paramString.length(),MPI_CHAR,slaveNumber,tag,MPI_COMM_WORLD);

}

void MPIModelInterface::receiveResultsFromSlave(int & taskRank, vector< ModelResults > & results) {

	int resultNumber;
	//int resultLength;
	//MPI_Status status;

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Waiting for  results from slave ... ";	
	mpiChannel.messageRank(MPI_ANY_SOURCE);
	mpiChannel >> resultNumber;
	taskRank = mpiChannel.messageRank();

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Receiving result " << resultNumber <<" from slave " << taskRank << "... ";	
	
	results[resultNumber].readFrom(mpiChannel);

	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Results received results" << endl;
	
	//MPI_Recv(&resultNumber,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	//taskRank = status.MPI_SOURCE;
	//MPI_Recv(&resultLength,1,MPI_INT,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		
	
	//char * resultString = new char[resultLength];
	//MPI_Recv(resultString,resultLength,MPI_CHAR,MPI_ANY_SOURCE,tag, MPI_COMM_WORLD, &status);		

	//cout << string(resultString) << endl;

	//istringstream resultStream; 
	//resultStream.str(string(resultString));
	
	//results[resultNumber].readFrom(resultStream);
	
	//delete resultString;
}

void MPIModelInterface::startSlave() {

	int resultNumber;
	//int parametersLength;
	//int resultsLength;
	MPI_Status status;
	
	while (true) {
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " is waiting for parameters from master ..." << endl;
		mpiChannel.messageRank(0);
		mpiChannel.messageId(MPI_ANY_TAG);
		mpiChannel >> resultNumber;
		//MPI_Recv(&resultNumber, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
		if (mpiChannel.messageId() == dietag) {
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " received a kill signal from master" << endl;
			return;
		}
		else if (mpiChannel.messageId() != tag) {
			crash("MPIModelInterface", "Slave "+str(rank)+" received message with wrong tag: "+str(status.MPI_TAG));
		}
		mpiChannel.messageId(tag);

		ModelTuningParameters parameters;
		parameters.readFrom(mpiChannel);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " running model with parameters: " << parameters.toString() << endl;

		ModelResults result = localModel->runModel(parameters);

		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " sending results back to master ... ";

		mpiChannel.messageRank(0);
		mpiChannel << resultNumber;
		result.printOn(mpiChannel);
		
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has sent results back" << endl;

		//MPI_Recv(&parametersLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status); 

		//char * parametersString = new char[parametersLength];
		//MPI_Recv(parametersString, parametersLength, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

		//if (toInt(fixedParams["VerboseLevel"]) > 4) cout << "Slave " << rank << " received parameters string from master: " << string(parametersString) <<  endl;

		//istringstream parametersStream;
		//parametersStream.str(string(parametersString));

		//ModelTuningParameters parameters;
		//parameters.readFrom(parametersStream);

		//if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " running model with parameters: " << parameters.toString() << endl;
	
		//ModelResults result = localModel->runModel(parameters);
		
		//ostringstream resultStream;
		//result.printOn(resultStream);
		//string resultString = resultStream.str();
		//resultsLength = resultString.length()+1;

		//if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " sending results back to master ... ";

		//MPI_Send(&resultNumber, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		//MPI_Send(&resultsLength, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
		//MPI_Send((void*)resultString.c_str(), resultsLength, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

		//if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has sent results back" << endl;
	
		//delete parametersString;
	}
	if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has stopped" << endl;
}


