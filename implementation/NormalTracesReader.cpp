/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include <math.h>

#include "../NormalTracesReader.h"

/// Read data traces from a directory 
ModelResults NormalTracesReader::readTraces(string dirName) {

	int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);

	// "results" will contain all the traces
	ModelResults results(getNumberOfRuns()*getNumberOfPeriods()*getNumberOfRecordSites());

	//////////////////////////////////////
    /// Read the weights for every run ///
    //////////////////////////////////////

	vector<double> runWeights(numberOfRuns,0);
	double dummy;

    istringstream runStream(fixedParams["RunParameters"]);
    for (int i = 0; i < numberOfRuns; i++) {
		// Skip the run parameters
        for (int j = 0; j < getNumberOfRunParameters(); j++) {
            runStream >> dummy;
        }
		if (!runStream.good()) crash("NormalTracesReader","Error while reading " + str(numberOfRuns) + " run weights  from parameter file");
        runStream >> runWeights[i];
    }

	for (int nRun = 0; nRun < numberOfRuns; nRun++) {

    	int numberOfRecordSites = toInt(fixedParams["NumberOfRecordSites"]);
    	int numberOfPeriods = toInt(fixedParams["NumberOfPeriods"]);

    	///////////////////////////////////////////////
    	/// Read the period start,stops and weights ///
    	///////////////////////////////////////////////
    	vector< double > periodStart(numberOfPeriods,0);
    	vector< double > periodStops(numberOfPeriods,0);
    	vector< double > periodWeights(numberOfPeriods,0);

    	istringstream periodStream(fixedParams["Periods"]);
    	for (int i = 0; i < numberOfPeriods; i++) {
			if (!periodStream.good()) crash("NormalTracesReader","Error while reading " + str(numberOfPeriods) + " recording periods from parameter file");
        	periodStream >> periodStart[i];
        	periodStream >> periodStops[i];
        	periodStream >> periodWeights[i];
    	}

    	///////////////////////////////////////
    	/// Read the recording site weights ///
    	///////////////////////////////////////
    	vector< double > recordWeights(numberOfRecordSites,0);
    	istringstream recordStream(fixedParams["RecordSites"]);
    	for (int i = 0; i < numberOfRecordSites; i++) {
			if (!periodStream.good()) crash("NormalTracesReader","Error while reading " + str(numberOfRecordSites) + " recording site weights from parameter file");
        	recordStream >> recordWeights[i];
    	}

    	////////////////////////////////////
    	/// Read every period seperately ///
    	////////////////////////////////////
    	for (int nPeriod = 0; nPeriod < numberOfPeriods; nPeriod++) {
        	//Calculate the index of this period in the 1 dimensional array of traces
        	int periodIndex = nRun*numberOfRecordSites*numberOfPeriods+nPeriod*numberOfRecordSites;

        	//////////////////////////
        	/// Open the data file ///
        	//////////////////////////
			string inputFileName = dirName + "/" + fixedParams["OutputFilePrefix"] + "_Run" + str(nRun) +".dat";
        	ifstream inputFile(inputFileName.c_str(), ios::in);
            if (!inputFile.good()) crash("NormalTracesReader","Error while opening file "+inputFileName);
        	showMessage("Reading from file: " + inputFileName + " from " + str(periodStart[nPeriod]) + " until " + str(periodStops[nPeriod]) + "\n",5,fixedParams);

        	///////////////////////////////////////////////
			/// Read data until the start of the period ///
        	///////////////////////////////////////////////
        	double time = 0;
        	double dummy;
			inputFile >> time;
			while (time < periodStart[nPeriod]) {
            	if (!inputFile.good()) crash("NormalTracesReader","Error while reading from file "+inputFileName+ " at time "+str(time));
                for (int nRecording = 0; nRecording < numberOfRecordSites; nRecording++) {
                    inputFile >> dummy;
                }
                inputFile >> time;
            }

        	/////////////////////////////
        	/// Initialize the traces ///
        	/////////////////////////////
        	for (int nRecording = 0; nRecording < numberOfRecordSites; nRecording++) {
            	results[periodIndex+nRecording].resetAndSetLength(
                	(int)floor(periodStops[nPeriod]*toDouble(fixedParams["SamplingFrequency"]))
                	- (int)floor(periodStart[nPeriod]*toDouble(fixedParams["SamplingFrequency"])) + 1);
            	results[periodIndex+nRecording].setName("Run: " + str(nRun) + ", Period: " + str(nPeriod) + ", Recording: "+str(nRecording));
            	results[periodIndex+nRecording].setSamplingFrequency(toDouble(fixedParams["SamplingFrequency"]));
            	results[periodIndex+nRecording].setWeight(runWeights[nRun]*periodWeights[nPeriod]*recordWeights[nRecording]);
            	results[periodIndex+nRecording].setStartTime(time);
            	results[periodIndex+nRecording].setStopTime(periodStops[nPeriod]);
        	}

        	/////////////////////
        	/// Read the data ///
        	/////////////////////
        	int i = 0;
        	while (time <= periodStops[nPeriod] && !inputFile.eof()) {
            	if (!inputFile.good()) crash("NormalTracesReader","Error while reading from file "+inputFileName+ " at time "+str(time));
            	for (int nRecording = 0; nRecording < numberOfRecordSites; nRecording++) {
                	inputFile >> results[periodIndex+nRecording][i++];
            	}
            	inputFile >> time;
        	}

        	inputFile.close();
		}

	}

	return results;

}
