#include "../NormalTracesReader.h"


ModelResults NormalTracesReader::readTraces(string dirName) {

	int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);

	ModelResults results(getNumberOfRuns()*getNumberOfPeriods()*getNumberOfRecordSites());

	//////////////////////////////////////
    /// Read the weights for every run ///
    //////////////////////////////////////

	vector<double> runWeights(numberOfRuns,0);
	double dummy;

    istringstream runStream(fixedParams["RunParameters"]);
    for (int i = 0; i < numberOfRuns; i++) {
        for (int j = 0; j < getNumberOfRunParameters(); j++) {
            runStream >> dummy;
        }
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
        	ifstream        inputFile;
        	inputFile.open(inputFileName.c_str(), ios::in);
        	if (toInt(fixedParams["VerboseLevel"]) > 4) {cout << "Reading from file: " << inputFileName << endl;};

        	///////////////////////////////////////////////
			/// Read data until the start of the period ///
        	///////////////////////////////////////////////
        	double time = 0;
        	char dummy[512];
        	inputFile >> time;
        	while (time < periodStart[nPeriod]) {
            	if (!inputFile.good()) crash("GenesisModelInterface","Error while reading file: "+inputFileName);
            	inputFile.getline(dummy,512);
            	inputFile >> time;
        	}

        	/////////////////////////////
        	/// Initialize the traces ///
        	/////////////////////////////
        	for (int nRecording = 0; nRecording < numberOfRecordSites; nRecording++) {
            	///todo check for rounding errors
            	results[periodIndex+nRecording].resetAndSetLength(
                	(int)(periodStops[nPeriod]*toDouble(fixedParams["SamplingFrequency"]))
                	- (int)(periodStart[nPeriod]*toDouble(fixedParams["SamplingFrequency"])) + 1);
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
        	while (time <= periodStops[nPeriod]) {
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
