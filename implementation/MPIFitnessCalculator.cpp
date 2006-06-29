#include "../MPIFitnessCalculator.h"

const int tag = 42;
const int dietag = 666; //Qui habet intellectum, computet numerum bestiae

MPIFitnessCalculator::MPIFitnessCalculator(ModelInterface * model, ExperimentInterface * experiment, FixedParameters params) 
	: FitnessCalculator(NULL), FixedParamObject(params) {

	FixedParameters fitFixedParams(fixedParams["FitnessCalculatorParameters"],fixedParams.getGlobals());
	if (fixedParams["FitnessCalculatorType"] == "Matrix") {
		localFitness = new MatrixFitnessCalculator(model,experiment,fitFixedParams);
	}
	else crash("MPIFitnessCalculator", "No matching fitness calculator type");
    
	mpiChannel.setMessageId(tag);
	rank = mpiChannel.getRank();
	ntasks = mpiChannel.getSize();

	if (rank != 0) startSlave();
	
	if (rank == 0) {                                
		if (toInt(fixedParams["enableFileExport"]) > 0) {
			this->enableFileExport(fixedParams["exportFile"]);
		}
	}
        
}

MPIFitnessCalculator::~MPIFitnessCalculator() {
	exportFileStream.close();
	if (rank == 0) {
		for (int i = 1; i < ntasks; ++i) {
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending kill signal to slave " << i << endl;
			mpiChannel.setMessageId(dietag);
			mpiChannel.setMessageRank(i);
			mpiChannel << 0;
		}
	}
	if (localFitness != NULL) delete localFitness;	                                                                        
}

/// todo don't return value, but use value inside ModelTuningParameters
double MPIFitnessCalculator::calculateFitness(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	return (calculateParallelFitness(paramList))[0];

}

vector< double > MPIFitnessCalculator::calculateParallelFitness(vector< ModelTuningParameters > & paramList) {

	vector< double > fitnessValues(paramList.size());

	if (toInt(fixedParams["VerboseLevel"]) > 2) cout << "Running "<< paramList.size() << " jobs in parallel" << endl;

	int nSubmitted = 0;
	int nReceived = 0;
	int taskRank = 1;

    //Run models on all available slaves
    while (nSubmitted < (int)ntasks-1 && nSubmitted < (int)paramList.size()) {
    	runFitnessOnSlave(taskRank++, nSubmitted, paramList[nSubmitted]);
        nSubmitted++;
    }

	//There are more jobs than slaves
	while (nSubmitted < (int)paramList.size()) {
		receiveFitnessFromSlave(taskRank, fitnessValues, paramList);
		nReceived++;
		runFitnessOnSlave(taskRank, nSubmitted ,paramList[nSubmitted]);
		nSubmitted++;
    }

    //Receive the remainder of the results
    while (nReceived < nSubmitted) {
       	receiveFitnessFromSlave(taskRank, fitnessValues, paramList);
        nReceived++;
    }

	numberOfGenerations++;

    return fitnessValues;

}


void MPIFitnessCalculator::runFitnessOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) {
    
    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Sending parameters to slave: " << slaveNumber << "... ";
    mpiChannel.setMessageRank(slaveNumber);
    mpiChannel << resultNumber;
    params.printOn(mpiChannel);
    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Parameters sent " << endl;

}

void MPIFitnessCalculator::receiveFitnessFromSlave(int & taskRank, vector< double > & results, vector< ModelTuningParameters > & paramList) {

    int resultNumber;

    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Waiting for fitness value from slave ... ";
    mpiChannel.setMessageRank(MPI_ANY_SOURCE);
    mpiChannel >> resultNumber;
    taskRank = mpiChannel.getMessageRank();

    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Receiving result " << resultNumber <<" from slave " << taskRank << "... ";

    mpiChannel >> results[resultNumber];
    paramList[resultNumber].readFrom(mpiChannel);
	numberOfEvaluations++;

	if (exportFileStream.is_open()) {
			exportFileStream << numberOfGenerations << " " << numberOfEvaluations << " " << results[resultNumber] << " ";
			for (int j = 0; j < paramList[resultNumber].getLength(); j++) {
				exportFileStream << (paramList[resultNumber][j]) << " ";
			}
			exportFileStream << endl;
   	}
                                                                        

    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << " Fitness value received" << endl;

}

void MPIFitnessCalculator::startSlave() {

    int resultNumber;
    MPI_Status status;

    while (true) {
        if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank <<  " is waiting for parameters from master ..." << endl;
        mpiChannel.setMessageRank(0);
        mpiChannel.setMessageId(MPI_ANY_TAG);
        mpiChannel >> resultNumber;
        if (mpiChannel.getMessageId() == dietag) {
            if (toInt(fixedParams["VerboseLevel"]) > 2) cout << "Slave " << rank <<  " received a kill signal from master" << endl;
            return;
        }
        else if (mpiChannel.getMessageId() != tag) {
            crash("MPIFitnessCalculator", "Slave "+str(rank)+" received message with wrong tag: "+str(status.MPI_TAG));
        }
        mpiChannel.setMessageId(tag);
            
        ModelTuningParameters parameters;
        parameters.readFrom(mpiChannel);

        if (toInt(fixedParams["VerboseLevel"]) > 2) cout << "Slave " << rank << " running model with parameters: " << parameters.toString() << endl;
    
        double result = localFitness->calculateFitness(parameters);
    
    	parameters.setFitnessValue(result);
    
        if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " sending fitness back to master ... ";
        
        mpiChannel.setMessageRank(0);
        mpiChannel << resultNumber;
        mpiChannel << result;
        parameters.printOn(mpiChannel);
        
        if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has sent fitness back" << endl;
            
    }
    if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "Slave " << rank << " has stopped" << endl;
}




vector< pair< ModelTuningParameters, double > > MPIFitnessCalculator::getFitnessHistory() {
	return fitnessHistory;
}


void MPIFitnessCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	if (toInt(fixedParams["VerboseLevel"]) > 2) {
		cout << "MPIFitnessCalculator: Enabled export to file: " << fileName << endl;
	}
}

void MPIFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MPIFitnessCalculator::getExportFile() const {
	return exportFile;
}
