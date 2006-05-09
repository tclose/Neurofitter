#include "../Evolufit.h"

using namespace std;

///todo check if all files are closed
///todo check for leaks
///todo add comments
///todo make all classes use initializers
///todo the correct header files to every header file
///todo check in every class to see if virtual is OK
///todo make it possible to fix some variables
///todo remove Houston

int main () {
	cout << "Houston, we have liftoff..." << endl;
	
	/////////////////////////
	/// Declare variables ///
	/////////////////////////
	ModelInterface * model;
	ExperimentInterface * experiment;
	FitnessCalculator * fitness;
	FitterInterface * fitter;

	///////////////////////////
	///	Read parameters file ///
	///////////////////////////

	// Read data from file
	ifstream paramFile("parameters.xml");
	string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
	FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

	// Say which parameters should be passed to child objects
	fixedParameters.setGlobal("Dimensions");
	fixedParameters.setGlobal("VerboseLevel");
	fixedParameters.setGlobal("SamplingFrequency");
	fixedParameters.setGlobal("Seed");
	fixedParameters.setGlobal("Bounds");

	ModelTuningParameters startPoint(fixedParameters["StartingPoints"],toInt(fixedParameters["Dimensions"]),fixedParameters["Bounds"]);

	if (toInt(fixedParameters["VerboseLevel"]) > 2) {
		cout << "VerboseLevel: " << fixedParameters["VerboseLevel"] << endl;
		cout << "Dimensions: " << fixedParameters["Dimensions"] << endl;
		cout << "Bounds: "<< fixedParameters["Bounds"] << endl;
		cout << "StartingPoints: " << fixedParameters["StartingPoints"] << endl;	
	}

	////////////////////////
	/// Initialize Model ///
	////////////////////////
	FixedParameters modelFixedParams(fixedParameters["ModelParameters"],fixedParameters.getGlobals());
	if (fixedParameters["ModelType"] == "Genesis") {
		model = new GenesisModelInterface(modelFixedParams);
	}
	else crash("Main program", "No matching model type");

	/////////////////////////////
	/// Initialize Experiment ///
	/////////////////////////////
	FixedParameters expFixedParams(fixedParameters["ExperimentParameters"],fixedParameters.getGlobals());
	if (fixedParameters["ExperimentType"] == "Fake") {
		experiment = new FakeExperimentInterface(model, expFixedParams);		
	}
	else crash("Main program", "No matching experiment type");

	/////////////////////////////////////
	/// Initialize Fitness Calculator ///
	/////////////////////////////////////
	FixedParameters fitFixedParams(fixedParameters["FitnessCalculatorParameters"],fixedParameters.getGlobals());
	if (fixedParameters["FitnessCalculatorType"] == "Pablo") {
		fitness = new PabloFitnessCalculator(model,experiment,fitFixedParams);
	}
	else crash("Main program", "No matching fitness calculator type");

	/////////////////////////
	/// Initialize Fitter ///
	/////////////////////////
	FixedParameters fitterFixedParams(fixedParameters["FitterParameters"],fixedParameters.getGlobals());
	if (fixedParameters["FitterType"] == "Pablo") {
		fitter = new PabloFitterInterface(fitness,fitterFixedParams);
	}
	else if (fixedParameters["FitterType"] == "Mesh") {
		fitter = new MeshFitterInterface(fitness,fitterFixedParams);
	}
	else if (fixedParameters["FitterType"] == "Swarm") {
		fitter = new SwarmFitterInterface(fitness,fitterFixedParams);
	}
	else if (fixedParameters["FitterType"] == "NOMAD") {
		fitter = new NOMADFitterInterface(fitness,fitterFixedParams);
	}
	else if (fixedParameters["FitterType"] == "EO") {
		fitter = new EOFitterInterface(fitness,fitterFixedParams);
	}
	else crash("Main program", "No matching fitter type");

	///////////
	/// Run ///
	///////////
	fitter->runFitter(&startPoint);


	///////////////
	/// Cleanup ///
	///////////////
	delete model;
	delete experiment;
	delete fitness;
	delete fitter;

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
