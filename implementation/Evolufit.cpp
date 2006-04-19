#include "../Evolufit.h"

using namespace std;

int main () {

	cout << "Houston, we have liftoff...\n" ;

	////
	//	Initialize
	////

	ifstream paramFile("parameters.xml");
	
	FixedParameters fixedParameters = FixedParameters(XMLString(string(istreambuf_iterator<char>(paramFile),istreambuf_iterator<char>())));

	FixedParameters globalParameters;
	globalParameters["Dimensions"] = fixedParameters["Dimensions"];

	cout << globalParameters["Dimensions"];

	double startPointArray [] = {1.0, 1.0};
	double bounds [] = {1.0, 10000.0, 1.0, 50000.0};

	ModelTuningParameters startPoint(startPointArray,2,bounds);

	double meshArray [] = {10, 10};
	ModelTuningParameters mesh(meshArray,2,bounds);

	WernerModelInterface model = WernerModelInterface(); 
	WernerExperimentInterface experiment(FixedParameters(fixedParameters["Experiment"],globalParameters));	

	PabloFitnessCalculator fitcal(&model,&experiment);

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
