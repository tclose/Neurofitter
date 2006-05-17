#include "../GenesisModelInterface.h"

ModelResults	GenesisModelInterface::
runModel(const ModelTuningParameters & params) {

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

		string paramFilename = fixedParams["ModelDirectory"] + "/" + fixedParams["ParamFilePrefix"] + ".dat";
    	paramFile.open(paramFilename.c_str(), ios::out);
		if (toInt(fixedParams["VerboseLevel"]) > 4) {cout << endl << "Writing data to parameter file: " << paramFilename << endl;}

		// put output filename in file //
		string modelOutputname = fixedParams["OutputFilePrefix"] + "_Run" + str(nRun) +".dat";
		paramFile << modelOutputname << endl;		
		
		// put run parameters in file //
		for (int i = 0; i < toInt(fixedParams["NumberOfRunParameters"]); i++) {
			paramFile << runParameters[nRun][i] << " ";
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
						+fixedParams["GenesisLocation"]+" -nox -notty -batch "+fixedParams["ModelSource"]+" > gen.out 2> " + errorFileName;
		if (toInt(fixedParams["VerboseLevel"]) > 3) {cout << endl << "calling " << genCommand << endl;}
		
		system(genCommand.c_str());

		/////////////////////////////////////////////////////
		/// Check to see if there was no error in genesis ///
		/////////////////////////////////////////////////////

		errorFile.open((fixedParams["ModelDirectory"]+"/"+errorFileName).c_str(), ios::in);
		errorFile.seekg (0, ios::end);
  		//int length = errorFile.tellg();
		//if (errorFile.bad() || length != 228) crash("GenesisModelInterface","Error while running genesis simulation");
		errorFile.close();

		////////////////////
		/// read results ///
		////////////////////
	
		readDataFromFile(results, fixedParams["ModelDirectory"]+"/"+modelOutputname, nRun, runWeights[nRun]);
	}

	//ostringstream output;
	//results.printOn(output);	

	//istringstream input;
	
	//string test = output.str();
	//cout << test << endl;
	//input.str(test);

	//ModelResults results2;
	//results2.readFrom(input);

	return results;
}


vector< ModelResults > GenesisModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	vector< ModelResults > resultList(paramList.size()); 

	for (unsigned int i = 0; i < paramList.size(); i++) {
		resultList[i] = runModel(paramList[i]);
	}

	return resultList;

}

void GenesisModelInterface::readDataFromFile(ModelResults & results, string inputFileName, int nRun, double runWeight) const {

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
		ifstream		inputFile;
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
           	results[periodIndex+nRecording].setWeight(runWeight*periodWeights[nPeriod]*recordWeights[nRecording]);
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
