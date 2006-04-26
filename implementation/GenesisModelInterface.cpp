#include "../GenesisModelInterface.h"

ModelResults	GenesisModelInterface::
runModel(const ModelTuningParameters & params) const {

	const int       numberOfRuns = toInt(fixedParams["NumberOfRuns"]);
	const int       numberOfRunParameters = toInt(fixedParams["NumberOfRunParameters"]);
	const int       numberOfRecordSites = toInt(fixedParams["NumberOfRecordSites"]);
	const int       numberOfPeriods = toInt(fixedParams["NumberOfPeriods"]);

	ofstream		paramFile;
	ofstream        fixedParamFile;

	ifstream		errorFile;

	ModelResults	results(numberOfRuns*numberOfPeriods*numberOfRecordSites);

	vector< vector< int > > runParameters(numberOfRuns, vector< int >(numberOfRunParameters,0));
	vector<double> runWeights(numberOfRuns,0);

	/////////////////////////////////////////////////////
	/// Read the parameters and weights for every run ///
	/////////////////////////////////////////////////////
	istringstream runStream(fixedParams["RunParameters"]);
	for (int i = 0; i < numberOfRuns; i++) {
		for (int j = 0; j < numberOfRunParameters; j++) {
			runStream >> runParameters[i][j];
		}
		runStream >> runWeights[i];
	}


	for (int nRun = 0; nRun < numberOfRuns; nRun++) {

		/////////////////////////////////
		/// Write the parameter file ///
		/////////////////////////////////

		string paramFilename = fixedParams["modelDirectory"] + "/" + fixedParams["ParamFilePrefix"] + ".dat";
    	paramFile.open(paramFilename.c_str(), ios::out);
		if (toInt(fixedParams["VerboseLevel"]) > 4) {cout << endl << "Writing data to parameter file: " << paramFilename << endl;}

		// put output filename in file //
		string modelOutputname = fixedParams["modelDirectory"] + fixedParams["OutputFilePrefix"] + "_" + str(nRun) +".dat";
		paramFile << modelOutputname << endl;		
		
		// put run parameters in file //
		for (int i = 0; i < toInt(fixedParams["NumberOfRunParameters"]); i++) {
			paramFile << runParameters[nRun][i] << endl;
		}
		paramFile << endl;

		// put tuning parameters in file //
		for (int i = 0; i < toInt(fixedParams["Dimensions"]); i++) {
			paramFile << params[i] << " ";
		}
		paramFile << endl;

    	paramFile.close ();

		// create or clean a file to store the genesis error channel
		string errorFileName = fixedParams["ErrorFile"];
		errorFile.open(errorFileName.c_str(), ios::trunc);
		errorFile.close();

		////////////////////
		/// call Genesis ///
		////////////////////

		string genCommand = "cd "+fixedParams["ModelDirectory"]+"; "
						+fixedParams["GenesisLocation"]+" -nox -notty -batch"+fixedParams["InputFile"]+" > gen.out 2> " + errorFileName;
		if (toInt(fixedParams["VerboseLevel"]) > 4) {cout << endl << "calling " << genCommand << endl;}
		
		system(genCommand.c_str());

		/////////////////////////////////////////////////////
		/// Check to see if there was no error in genesis ///
		/////////////////////////////////////////////////////

		errorFile.open(errorFileName.c_str(), ios::in);
		errorFile.seekg (0, ios::end);
  		int length = errorFile.tellg();
		if (errorFile.bad() || length != 228) {cerr << "Error while running genesis simulation in GenesisModeInterface" << endl;exit(1);}
		errorFile.close();

		////////////////////
		/// read results ///
		////////////////////
	
		readDataFromFile(results, modelOutputname, nRun, runWeights[nRun]);

	}

	return results;
}


void GenesisModelInterface::readDataFromFile(ModelResults & results, string inputFileName, double runWeight) {

	int numberOfRecordSites = toInt(fixedParams["NumberOfRecordSites"]);
	int numberOfPeriods = toInt(fixedParams["NumberOfPeriods"]);

	///////////////////////////////////////////////
	/// Read the period start,stops and weights ///
	///////////////////////////////////////////////
	double * periodStart[] = double [numberOfPeriods];
	double * periodStops[] = double [numberOfPeriods];
	double * periodWeights[] = double [numberOfPeriods];

	istringstream periodStream(fixedParams["Periods"]);
	for (int i = 0; i < numberOfPeriods; i++) {
		periodStream >> periodStart[i];
		periodStream >> periodStop[i];
		periodStream >> periodWeights[i];
	}

	///////////////////////////////////////
	/// Read the recording site weights ///
	///////////////////////////////////////
	double * recordWeights[] = double [numberOfRecordSites];
	istringstream recordingStream(fixedParams["RecordSites"]);
	for (int i = 0; i < numberOfRecordSites; i++) {
		recordStream >> recordWeights[i];
	}


	////////////////////////////////////
	/// Read every period seperately ///
	////////////////////////////////////
	for (int nPeriod = 0; nPeriod < toInt(numberOfPeriods); nPeriod++) {
		//Calculate the index of this period in the 1 dimensional array of traces
		int periodIndex = nInject*numberOfRecordSites*numberOfPeriods+nPeriod*numberOfRecordSites;

		//////////////////////////
		/// Open the data file ///
		//////////////////////////
		ifstream		inputfile;
    	inputfile.open(inputFileName.c_str(), ios::in);
     	inputfile.clear();

		///////////////////////////////////////////////
		/// Read data until the start of the period ///
		///////////////////////////////////////////////
		double time = 0;
		inputfile >> time;
		while (time < periodStart[nPeriod]) {
			inputfile.getLine();
			inputfile >> time;
		}

		/////////////////////////////
		/// Initialize the traces ///
		/////////////////////////////
		for (int nRecording = 0; j < numberOfRecordSites; nRecording++) {
			///todo check for rounding errors
			results[periodIndex+nRecording].resetAndSetLength(
				(int)periodStop[nPeriod]*toDouble(fixedParam["SamplingFrequency"]) 
				- (int)periodStart[nPeriod]*toDouble(fixedParam["SamplingFrequency"]) + 1);
           	results[periodIndex+nRecording].setName("Run: " + str(nRun) + ", Period: " + str(nPeriod) + ", Recording: "+str(nRecord));
           	results[periodIndex+nRecording].setSamplingFrequency(toDouble(fixedParam["SamplingFrequency"]));
           	results[periodIndex+nRecording].setWeight(runWeight*periodWeigths[nPeriod]*recordWeights[nRecording]);
			results[periodIndex+nRecording].setStartTime(time);
			results[periodIndex+nRecording].setStopTime(periodStop[nPeriod]);
		}	

		/////////////////////
		/// Read the data ///
		/////////////////////
		int i = 0;
		while (time <= periodStop[nPeriod]) {
			for (int nRecording = 0; j < numberOfRecordSites; nRecording++) {
           		inputFile >> results[periodIndex+nRecord][i++];
			}
			inputfile >> time;
		}

		inputfile.close();
	}

}
