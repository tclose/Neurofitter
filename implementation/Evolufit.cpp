/*
* Algorithm
*
*
* read evolufit parameters
*
* read starting points
*
* create fitness calculator
* 
* start EO
*	give parameters
*	give starting points
*	give fitness calculator
* get EO information
* get EO result
*
*
* start NOMAD
*	give parameters
*	give starting point from EO
*	give fitness calculator
* get NOMAD information
* get NOMAD result
*
* write evolufit results / fitting info
*
*/

#include <iostream>
#include "../Evolufit.h"

using namespace std;

extern void setNOMADFitnessCalculator(FitnessCalculator*);

int main () {

	cout << "Houston, we have liftoff...\n" ;

	////
	//	Initialize
	////
	
	WernerExperimentInterface experiment = WernerExperimentInterface();	
/*
	double testSet1 [] = {500,15,15000,50};
	ModelTuningParameters modelparams1(testSet1,4);
*/
	double initParamsArray [] = {6000.0, 6.0, 600.0,1000.0};
	//double bounds [] = {70000.0, 80000.0, 1.0, 20.0, 100.0, 200.0, 4000.0, 6000.0};
	double bounds [] = {1.0, 100000.0, 1.0, 20.0, 1.0, 1000.0, 1.0, 10000.0};

	ModelTuningParameters initParams(initParamsArray,4,bounds);


	WernerModelInterface model = WernerModelInterface(); 

/*
	test = model.runModel(modelparams);

	for (int i = 0; i < test.getLength(); i++) {
		cout 	<< test[i].getName() << endl;
		cout 	<< "StartTime: " << str(test[i].getStartTime()) << " StopTime: " << str(test[i].getStopTime()) << endl; 
		cout	<< "Weight: " << str(test[i].getWeight()) << " Sampling Frequency: " << str(test[i].getSamplingFrequency());
		for (int j = 0; j < test[i].getLength(); j++) {
			if (j > 10) break;
			cout << " "<< test[i][j];
		}
		cout << endl;
	}
	

*/  

	PabloFitnessCalculator fitcal(&model, &experiment); 


/*
	cout << endl << "Fitness value: " << fitcal.calculateFitness(modelparams1) << endl;

	cout << endl << "Fitness value: " << fitcal.calculateFitness(modelparams2) << endl;

*/	
	EOFitterInterface eo = EOFitterInterface();

	eo.runFitter(&fitcal, &initParams, 5);

/*	NOMADFitterInterface nomad = NOMADFitterInterface();
	setNOMADFitnessCalculator(&fitcal);
	nomad.runFitter(&fitcal, &initParams, 5);
*/

	////
	//	Run EO
	////
		

	////
	// Run NOMAD
	////


	////
	// Finish up
	////

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
