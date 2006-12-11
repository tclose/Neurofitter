/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../ExecutableModelInterface.h"

ModelResults	ExecutableModelInterface::
runModel(const ModelTuningParameters & params) {

	const int       numberOfRuns = tracesReader->getNumberOfRuns();
	const int       numberOfRunParameters = tracesReader->getNumberOfRunParameters();

	ofstream		paramFile;
	ofstream        fixedParamFile;

	vector< vector< double > > runParameters(numberOfRuns, vector< double >(numberOfRunParameters,0));
	vector<double> runWeights(numberOfRuns,0);

	showMessage("Running model with parameters: " + params.toString(),4,fixedParams);

	flushMessage();
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

		string paramFilename = fixedParams["ParameterFile"];
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

		///////////////////////
		/// Execute command ///
		///////////////////////

		string execCommand = fixedParams["ExecuteCommand"];
		if (toInt(fixedParams["ShowExecuteOutput"]) == 0) execCommand += " >modelexec.out 2>modelexec.err";
		showMessage("calling " + execCommand + "\n",4,fixedParams);
				
		int exitCode = system(execCommand.c_str());

		if (exitCode == -1) crash("ExecutableModelInterface","Model execute command returned with error code");
		
	}

	////////////////////
	/// read results ///
	////////////////////

	return tracesReader->readTraces(fixedParams["OutputDirectory"]);

}


vector< ModelResults > ExecutableModelInterface::runParallelModel(const vector< ModelTuningParameters > paramList) {

	vector< ModelResults > resultList(paramList.size()); 

	for (unsigned int i = 0; i < paramList.size(); i++) {
		resultList[i] = runModel(paramList[i]);
	}

	return resultList;

}

