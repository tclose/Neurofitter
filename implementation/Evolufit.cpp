#include "../Evolufit.h"

using namespace std;

int main () {
	cout << "Houston, we have liftoff..." << endl;

	////////////////////
	///	Declarations ///
	////////////////////
	int dimensions;
	int verboseLevel;
	string bounds;
	string startingPoints;

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

	// Write parameters to variables
	verboseLevel = toInt(fixedParameters["VerboseLevel"]);
	dimensions = toInt(fixedParameters["Dimensions"]);
	bounds = fixedParameters["Bounds"];
	startingPoints = fixedParameters["StartingPoints"];

	if (toInt(fixedParameters["VerboseLevel"]) > 2) {
		cout << "VerboseLevel: " << verboseLevel << endl;
		cout << "Dimensions: " << dimensions << endl;
		cout << "Bounds: "<< bounds << endl;
		cout << "StartingPoints: " << startingPoints << endl;	
	}

	// Make the fixed parameters for the child objects
	FixedParameters expFixedParams(fixedParameters["FakeExperiment"],fixedParameters.getGlobals());
	FixedParameters modelFixedParams(fixedParameters["GenesisModel"],fixedParameters.getGlobals());
	FixedParameters fitFixedParams(fixedParameters["PabloFitnessCalculator"],fixedParameters.getGlobals());
	FixedParameters fitterFixedParams(fixedParameters["MeshFitter"],fixedParameters.getGlobals());

	//////////////////////////
	/// Initialize objects ///
	//////////////////////////

	ModelTuningParameters startPoint(startingPoints,dimensions,bounds);

	GenesisModelInterface model = GenesisModelInterface(modelFixedParams);
	FakeExperimentInterface experiment(&model, expFixedParams);
	PabloFitnessCalculator fitcal(&model,&experiment,fitFixedParams);

	MeshFitterInterface fitter = MeshFitterInterface(&fitcal,fitterFixedParams);
	//SwarmFitterInterface fitter = SwarmFitterInterface(&fitcal,5,10);
	//PabloFitterInterface fitter(&fitcal);
	//NOMADFitterInterface fitter(&fitcal, startPoint.getLength());
	//EOFitterInterface fitter(&fitcal);

	fitter.runFitter(&startPoint, toInt(fixedParameters["Seed"]));

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
