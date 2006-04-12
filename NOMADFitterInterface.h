#ifndef NOMADFITTERINTERFACE_H
#define NOMADFITTERINTERFACE_H

#include <nomad>
#include "FitterInterface.h"
#include "FitnessCalculator.h"

extern void setNOMADFitnessCalculator(FitnessCalculator*);

class NOMADFitterInterface : FitterInterface {

	public:
		NOMADFitterInterface(FitnessCalculator * fit):FitterInterface(fit) {setNOMADFitnessCalculator(fit);};
		////
		//Inherited from FitterInterface
    	////
		///todo check in every class to see if virtual is OK
		virtual FitterResults runFitter(ModelTuningParameters * startPt, int seed);

	private:
    	FitnessCalculator * fitness;

		void solveProblem(const Description & description,
      					const Parameters & parameters, Preferences & preferences,
          				Solver & solver);

		void writeStartingPointsFile(string, ModelTuningParameters&);
    	void writeBoundsFile(string, ModelTuningParameters&);
    	void writeParametersFile(string, int seed);
    	//void writeDescriptionFile(string);
    	//void writePreferencesFile(string);

};

#endif
