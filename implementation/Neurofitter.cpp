#include "../Neurofitter.h"

using namespace std;

///todo check if all files are closed
///todo check for leaks
///todo add comments
///todo make all classes use initializers
///todo the correct header files to every header file
///todo check in every class to see if virtual is OK
///todo make it possible to fix some variables
///todo make separate executable for mpi
///todo report bug in EO
///todo extend the members of FitterResults 
///todo make it possible to give a list of startingpoints to a fitter
///todo make it possible to make a slice in a certain dimension with MeshFitter
///todo put include Tools.h in implementation files, not header files except when necessary
///todo change nomad to prevent LIBRARY directory
///todo numberOfParameters in truthfunction of nomad part is not well coded (static)
///todo check the directory sign differences on unix/windows
///todo check 2>neuron.err on windows

FixedParameters readParameters(int argc, char* argv[]);

int main (int argc, char* argv[]) {

	showMessage("\nStarting Neurofitter...\n");
	showMessage("The date is: " + getDateAndTime() + "\n");		
		////////////////////////////
		///	Read parameters file ///
		////////////////////////////
		FixedParameters fixedParams = readParameters(argc,argv);

		//Set invalid rank, only used for parallel version
		fixedParams.addParameter("Rank",str(-1),1);
		
		/////////////////////////
    	/// Declare variables ///
    	/////////////////////////
    	TracesReader * tracesReader = NULL;
    	ModelInterface * model = NULL;
    	ExperimentInterface * experiment = NULL;
    	FitnessCalculator * fitness = NULL;
    	FitterInterface * fitter = NULL;

    	ModelTuningParameters startPoint(fixedParams["StartingPoints"],toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);


		///////////////////////////////
		/// Initialize TracesReader ///
		///////////////////////////////
		FixedParameters tracesReaderFixedParams(fixedParams["TracesReaderParameters"],fixedParams.getGlobals());
		if (fixedParams["TracesReaderType"] == "Normal") {
			tracesReader = new NormalTracesReader(tracesReaderFixedParams);
		}
		else crash("Main program", "No matching trace reader type");


		////////////////////////
		/// Initialize Model ///
		////////////////////////
		FixedParameters modelFixedParams(fixedParams["ModelParameters"],fixedParams.getGlobals());
		if (fixedParams["ModelType"] == "Genesis") {
			model = new GenesisModelInterface(tracesReader,modelFixedParams);
		}
		else if (fixedParams["ModelType"] == "Neuron") {
			model = new NeuronModelInterface(tracesReader,modelFixedParams);
		}
		else if (fixedParams["ModelType"] == "Executable") {
			model = new ExecutableModelInterface(tracesReader,modelFixedParams);
		}
		else crash("Main program", "No matching model type");
	
		/////////////////////////////
		/// Initialize Experiment ///
		/////////////////////////////
		FixedParameters expFixedParams(fixedParams["ExperimentParameters"],fixedParams.getGlobals());
		if (fixedParams["ExperimentType"] == "Fake") {
			experiment = new FakeExperimentInterface(model, expFixedParams);
		}
		else if (fixedParams["ExperimentType"] == "File") {
			experiment = new FileExperimentInterface(tracesReader, expFixedParams);
		}
		else crash("Main program", "No matching experiment type");

		/////////////////////////////////////
		/// Initialize Fitness Calculator ///
		/////////////////////////////////////
		FixedParameters fitFixedParams(fixedParams["FitnessCalculatorParameters"],fixedParams.getGlobals());
		if (fixedParams["FitnessCalculatorType"] == "Matrix") {
			fitness = new MatrixFitnessCalculator(model,experiment,fitFixedParams);
		}
		else crash("Main program", "No matching fitness calculator type");
	

		/////////////////////////
		/// Initialize Fitter ///
		/////////////////////////
		FixedParameters fitterFixedParams(fixedParams["FitterParameters"],fixedParams.getGlobals());
		if (fixedParams["FitterType"] == "Mesh") {
			fitter = new MeshFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "Easy") {
			fitter = new EasyFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "Random") {
			fitter = new RandomFitterInterface(fitness,fitterFixedParams);
		}
		else if (fixedParams["FitterType"] == "Swarm") {
			fitter = new SwarmFitterInterface(fitness,fitterFixedParams);
		}
	#ifdef WITH_NOMAD
		else if (fixedParams["FitterType"] == "NOMAD") {
			fitter = new NOMADFitterInterface(fitness,fitterFixedParams);
		}
	#endif
	#ifdef WITH_EO
		else if (fixedParams["FitterType"] == "EO") {
			fitter = new EOFitterInterface(fitness,fitterFixedParams);
		}
	#endif
	#ifdef WITH_EO
		#ifdef WITH_NOMAD
		else if (fixedParams["FitterType"] == "EONOMAD") {
			fitter = new EONOMADFitterInterface(fitness,fitterFixedParams);
		}
		#endif
	#endif 
		else crash("Main program", "No matching fitter type");

		///////////
		/// Run ///
		///////////
		FitterResults results = fitter->runFitter(&startPoint);

		showMessage("\nBest fit found: "+ results.getBestFit().toString() + " with fitness: " + str(results.getBestFitness()) +"\n",1,fixedParams);

		///////////////
		/// Cleanup ///
		///////////////
		delete fitter;
		delete experiment;
		delete fitness;
		delete model;

	showMessage("\nNeurofitter has finished\n");

	return 0;

}


FixedParameters readParameters(int argc, char* argv[]) {

	showMessage("Arguments: {");
	for (int i = 0; i < argc; i++) {
		showMessage(string(argv[i]) + ",");
	}
	showMessage("}\n");
	if (argc < 2 || argv[1]==NULL) crash("Neurofitter","Not enough arguments: "+str(argc));
	
	ifstream paramFile(argv[1]);
	string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
	fileContent = XMLString::removeXMLComments(fileContent);
	FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

	// Say which parameters should be passed to child objects
	fixedParameters.setGlobal("Dimensions");
	fixedParameters.setGlobal("VerboseLevel");
	fixedParameters.setGlobal("SamplingFrequency");
	fixedParameters.setGlobal("Seed");
	fixedParameters.setGlobal("Bounds");
	fixedParameters.setGlobal("WorkingDirectory");

    if (toInt(fixedParameters["PrintParameterFile"]) >= 1) {
		showMessage("Parameter file: \n" + fileContent + "\n", 1, fixedParameters);
	}

	showMessage("VerboseLevel: " + fixedParameters["VerboseLevel"] + "\n" +
        		"Dimensions: " + fixedParameters["Dimensions"] + "\n" +
        		"Bounds: " + fixedParameters["Bounds"] + "\n" + 
        		"StartingPoints: " + fixedParameters["StartingPoints"] + "\n" + 
       			"Sampling Frequency: " + fixedParameters["SamplingFrequency"] + "\n" + 
        		"Seed: " + fixedParameters["Seed"] + "\n" + 
        		"Working Directory: " + fixedParameters["WorkingDirectory"] + "\n", 
				3, fixedParameters);

	return fixedParameters;
	
}
