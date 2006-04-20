#include "../Evolufit.h"

using namespace std;

int main () {
	/////
	//	Declarations
	////
	int dimensions;
	int verboseLevel;
	string bounds;
	string startingPoints;

	cout << "Houston, we have liftoff...\n" ;

	///////////////////////////
	///	Read parameter file ///
	///////////////////////////

	ifstream paramFile("parameters.xml");
	string fileContent = string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>());
	FixedParameters fixedParameters = FixedParameters(XMLString("<root>"+fileContent+"</root>").getSubString("TestProgram"));

	FixedParameters globalParameters;
		globalParameters["Dimensions"] = fixedParameters["Dimensions"];
		globalParameters["VerboseLevel"] = fixedParameters["VerboseLevel"];
		globalParameters["Bounds"] = fixedParameters["Bounds"];

	verboseLevel = atoi(fixedParameters["VerboseLevel"].c_str());
	dimensions = atoi(fixedParameters["Dimensions"].c_str());
	bounds = fixedParameters["Bounds"];
	startingPoints = fixedParameters["StartingPoints"];	

	cout << globalParameters.toString();

	FixedParameters expFixedParams(fixedParameters["WernerExperiment"],globalParameters);
	FixedParameters fitFixedParams(fixedParameters["PabloFitnessCalculator"],globalParameters);

	//////////////////////////
	/// Initialize objects ///
	//////////////////////////

	ModelTuningParameters startPoint(startingPoints,dimensions,bounds);

	WernerModelInterface model = WernerModelInterface();
	WernerExperimentInterface experiment(expFixedParams);	
	PabloFitnessCalculator fitcal(&model,&experiment,fitFixedParams);

	//string meshArray = "10 10";
	//ModelTuningParameters mesh(meshArray,2,bounds);



	//fitcal.enableFileExport("fitness.dat");

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
