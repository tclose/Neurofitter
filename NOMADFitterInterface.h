#ifndef NOMADFITTERINTERFACE_H
#define NOMADFITTERINTERFACE_H

#include <nomad>
#include "FitterInterface.h"
#include "FitnessCalculator.h"

class NOMADFitterInterface : FitterInterface {

	public:
		////
		//Inherited from FitterInterface
    	////
		void runFitter(FitnessCalculator *, ModelTuningParameters * startPt, int seed);

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
