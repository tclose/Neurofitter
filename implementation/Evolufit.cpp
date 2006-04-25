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
	fixedParameters.setGlobal("Bounds");

	// Write parameters to variables
	cout << "VerboseLevel: " << (verboseLevel = toInt(fixedParameters["VerboseLevel"])) << endl;
	cout << "Dimensions: " << (dimensions = toInt(fixedParameters["Dimensions"])) << endl;
	cout << "Bounds: "<< (bounds = fixedParameters["Bounds"]) << endl;
	cout << "StartingPoints: " << (startingPoints = fixedParameters["StartingPoints"]) << endl;	

	// Make the fixed parameters for the child objects
	FixedParameters expFixedParams(fixedParameters["WernerExperiment"],fixedParameters.getGlobals());
	FixedParameters modelFixedParams(fixedParameters["WernerModel"],fixedParameters.getGlobals());
	FixedParameters fitFixedParams(fixedParameters["PabloFitnessCalculator"],fixedParameters.getGlobals());

	//////////////////////////
	/// Initialize objects ///
	//////////////////////////

	ModelTuningParameters startPoint(startingPoints,dimensions,bounds);

	WernerModelInterface model = WernerModelInterface(modelFixedParams);
	WernerExperimentInterface experiment(&model, expFixedParams);
	PabloFitnessCalculator fitcal(&model,&experiment,fitFixedParams);

	//string meshArray = "10 10";
	//ModelTuningParameters mesh(meshArray,2,bounds);


	//MeshFitterInterface fitter = MeshFitterInterface(&fitcal);
	//fitter.runFitter(&mesh, 1550);


	//SwarmFitterInterface fitter = SwarmFitterInterface(&fitcal,5,10);
	//PabloFitterInterface fitter(&fitcal);
	//NOMADFitterInterface fitter(&fitcal, startPoint.getLength());
	//EOFitterInterface fitter(&fitcal);

	//fitter.runFitter(&startPoint, 1550);

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
