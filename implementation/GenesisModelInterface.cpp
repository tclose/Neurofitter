#include "../GenesisModelInterface.h"

ModelResults	GenesisModelInterface::
runModel(const ModelTuningParameters & params) {

	const int       numberOfRuns = tracesReader->getNumberOfRuns();
	const int       numberOfRunParameters = tracesReader->getNumberOfRunParameters();

	ofstream		paramFile;
	ofstream        fixedParamFile;

	ifstream		errorFile;

	vector< vector< double > > runParameters(numberOfRuns, vector< double >(numberOfRunParameters,0));
	vector<double> runWeights(numberOfRuns,0);

	/////////////////////////////////////////////////////
	/// Read the parameters and weights for every run ///
	/////////////////////////////////////////////////////
	istringstream runStream(tracesReader->getRunParameters());
	showMessage(" and run parameters: ",4,fixedParams);
	for (int i = 0; i < numberOfRuns; i++) {
		showMessage("{",4,fixedParams);
		for (int j = 0; j < numberOfRunParameters; j++) {
			runStream >> runParameters[i][j];
			showMessage(" " + str(runParameters[i][j])  + " ",4,fixedParams);
		}
		runStream >> runWeights[i];
		showMessage("} ",4,fixedParams);
	}
	showMessage("\n",3,fixedParams);


	for (int nRun = 0; nRun < numberOfRuns; nRun++) {

		/////////////////////////////////
		/// Write the parameter file ///
		/////////////////////////////////

		string paramFilename = fixedParams["ModelDirectory"] + "/" + fixedParams["ParamFilePrefix"] + ".dat";
    	paramFile.open(paramFilename.c_str(), ios::out);
		showMessage("\nWriting data to parameter file: " + paramFilename + "\n",5,fixedParams);

		// put output filename in file //
		string modelOutputname = fixedParams["OutputDirectory"] + "/" + tracesReader->getOutputFilePrefix() + "_Run" + str(nRun) +".dat";
		paramFile << modelOutputname << endl;		
		
		// put run parameters in file //
		for (int i = 0; i < tracesReader->getNumberOfRunParameters(); i++) {
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
		showMessage("calling " + genCommand + "\n",4,fixedParams);
				
		int exitCode = system(genCommand.c_str());

		if (exitCode == -1) crash("GenesisModelInterface","Genesis returned with error code");
		
		///////////////////////////////////////////////////////////////////////////////
		/// Check to see if there was no error in genesis (not used at this moment) ///
		///////////////////////////////////////////////////////////////////////////////

		errorFile.open((fixedParams["ModelDirectory"]+"/"+errorFileName).c_str(), ios::in);
		errorFile.seekg (0, ios::end);
  		//int length = errorFile.tellg();
		//if (errorFile.bad() || length != 228) crash("GenesisModelInterface","Error while running genesis simulation");
		errorFile.close();

	}

	////////////////////
	/// read results ///
	////////////////////

	return tracesReader->readTraces(fixedParams["OutputDirectory"]);

}


vector< ModelResults > GenesisModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	vector< ModelResults > resultList(paramList.size()); 

	for (unsigned int i = 0; i < paramList.size(); i++) {
		resultList[i] = runModel(paramList[i]);
	}

	return resultList;

}

