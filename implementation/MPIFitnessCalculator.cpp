/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

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
			showMessage("Sending kill signal to slave " + str(i) + "\n",4,fixedParams);
			mpiChannel.setMessageId(dietag);
			mpiChannel.setMessageRank(i);
			mpiChannel << 0;
		}
	}
	if (localFitness != NULL) delete localFitness;	                                                                        
}

void MPIFitnessCalculator::calculateFitness(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	calculateParallelFitness(paramList);

	/// This is necessary because otherwise the fitnessvalue is not transferred
	/// since no reference is passed to calculateParallelFitness
	params.setFitnessValue(paramList[0].getFitnessValue());

}

void MPIFitnessCalculator::calculateParallelFitness(vector< ModelTuningParameters > & paramList) {

	showMessage("Running a total of "+ str((int)paramList.size()) + " jobs on " + str(ntasks-1) + " parallel processors\n",3,fixedParams);

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
		receiveFitnessFromSlave(taskRank, paramList);
		nReceived++;
		runFitnessOnSlave(taskRank, nSubmitted ,paramList[nSubmitted]);
		nSubmitted++;
    }

    //Receive the remainder of the results
    while (nReceived < nSubmitted) {
       	receiveFitnessFromSlave(taskRank, paramList);
        nReceived++;
    }

	numberOfGenerations++;

}


void MPIFitnessCalculator::runFitnessOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params) {
    
    showMessage("Sending parameters to slave: " + str(slaveNumber) + "... ",4,fixedParams);
    mpiChannel.setMessageRank(slaveNumber);
    mpiChannel << resultNumber;
    params.printOn(mpiChannel);
    showMessage(" Parameters sent \n",4,fixedParams);

}

void MPIFitnessCalculator::receiveFitnessFromSlave(int & taskRank, vector< ModelTuningParameters > & paramList) {

    int resultNumber;

    showMessage("Waiting for fitness value from slave ... \n",4,fixedParams);
    mpiChannel.setMessageRank(MPI_ANY_SOURCE);
    mpiChannel >> resultNumber;
    taskRank = mpiChannel.getMessageRank();

    showMessage("Receiving result " + str(resultNumber) + " from slave " + str(taskRank) + "... ",4,fixedParams);

    paramList[resultNumber].readFrom(mpiChannel);
	numberOfEvaluations++;

	if (exportFileStream.is_open()) {
			exportFileStream << numberOfGenerations << " " << numberOfEvaluations << " " << paramList[resultNumber].getFitnessValue() << " ";
			for (int j = 0; j < paramList[resultNumber].getLength(); j++) {
				exportFileStream << (paramList[resultNumber][j]) << " ";
			}
			exportFileStream << endl;
   	}
                                                                        
    showMessage(" Fitness value received\n",4,fixedParams);

}

void MPIFitnessCalculator::startSlave() {

    int resultNumber;
    MPI_Status status;

    while (true) {
    	showMessage("Slave " + str(rank) +  " is waiting for parameters from master ...\n",4,fixedParams);
        mpiChannel.setMessageRank(0);
        mpiChannel.setMessageId(MPI_ANY_TAG);
        mpiChannel >> resultNumber;
        if (mpiChannel.getMessageId() == dietag) {
        	showMessage("Slave " + str(rank) +  " received a kill signal from master\n",3,fixedParams);
            return;
        }
        else if (mpiChannel.getMessageId() != tag) {
            crash("MPIFitnessCalculator", "Slave "+str(rank)+" received message with wrong tag: "+str(status.MPI_TAG));
        }
        mpiChannel.setMessageId(tag);
            
        ModelTuningParameters parameters;
        parameters.readFrom(mpiChannel);

        showMessage("Slave " + str(rank) + " running model with parameters: " + parameters.toString() + "\n",3,fixedParams);
    
        localFitness->calculateFitness(parameters);

        showMessage("Slave " + str(rank) + " sending fitness back to master ... ",4,fixedParams);
                
        mpiChannel.setMessageRank(0);
        mpiChannel << resultNumber;
        parameters.printOn(mpiChannel);
        
        showMessage("Slave " + str(rank) + " has sent fitness back\n",4,fixedParams);            
    }
    
    showMessage("Slave " + str(rank) + " has stopped\n",4,fixedParams);            
}




vector< pair< ModelTuningParameters, double > > MPIFitnessCalculator::getFitnessHistory() {
	return fitnessHistory;
}


void MPIFitnessCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	
	showMessage("MPIFitnessCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);            
}

void MPIFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MPIFitnessCalculator::getExportFile() const {
	return exportFile;
}
