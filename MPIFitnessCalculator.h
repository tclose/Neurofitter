/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MPIFITNESSCALCULATOR_H
#define NEUROFITTER_MPIFITNESSCALCULATOR_H

#include <iostream>
#include <fstream>
#include <vector>
#include <mpi.h>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ModelTuningParameters.h"
#include "FitnessCalculator.h"
#include "MatrixFitnessCalculator.h"
#include "FixedParamObject.h"
#include "MPIStream.h"

class MPIFitnessCalculator : public FitnessCalculator, public FixedParamObject {

public:
	MPIFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~MPIFitnessCalculator();

	virtual void calculateFitness(ModelTuningParameters & param);
	virtual void calculateParallelFitness(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory();

private:
	int rank, ntasks;
	MPIStream mpiChannel;
	ExperimentInterface * localExperiment;
	FitnessCalculator * localFitness;

	ofstream exportFileStream;

	void runFitnessOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params);
	void receiveFitnessFromSlave(int & taskRank, vector< ModelTuningParameters > & paramList);
	void startSlave();
	        
};

#endif
