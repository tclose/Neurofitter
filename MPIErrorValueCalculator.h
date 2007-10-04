/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MPIERRORVALUECALCULATOR_H
#define NEUROFITTER_MPIERRORVALUECALCULATOR_H

#include <mpi.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "ModelInterface.h"
#include "ExperimentInterface.h"
#include "ModelTuningParameters.h"
#include "ErrorValueCalculator.h"
#include "MatrixErrorValueCalculator.h"
#include "FixedParamObject.h"
#include "MPIStream.h"

class MPIErrorValueCalculator : public ErrorValueCalculator, public FixedParamObject {

public:
	MPIErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters); 
	~MPIErrorValueCalculator();

	virtual void calculateErrorValue(ModelTuningParameters & param);
	virtual void calculateParallelErrorValue(vector< ModelTuningParameters > & params);

	virtual void enableFileExport(const string fileName);
    virtual void disableFileExport();
    virtual string getExportFile() const;

	virtual vector< pair< ModelTuningParameters, double > > getErrorValueHistory();

private:
	int rank, ntasks;
	MPIStream mpiChannel;
	ExperimentInterface * localExperiment;
	ErrorValueCalculator * localErrorValue;

	ofstream exportFileStream;

	void runErrorValueOnSlave(int slaveNumber, int resultNumber, const ModelTuningParameters params);
	void receiveErrorValueFromSlave(int & taskRank, vector< ModelTuningParameters > & paramList);
	void startSlave();
	        
};

#endif
