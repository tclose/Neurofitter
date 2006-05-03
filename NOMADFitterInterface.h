#ifndef NOMADFITTERINTERFACE_H
#define NOMADFITTERINTERFACE_H

#include <nomad>
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

using namespace std;

extern void setNOMADFitnessCalculator(FitnessCalculator*, int);

class NOMADFitterInterface : public FitterInterface, FixedParamObject {

	public:
		NOMADFitterInterface(FitnessCalculator * fit, FixedParameters params);
		////
		//Inherited from FitterInterface
    	////
		virtual FitterResults runFitter(ModelTuningParameters * startPt);

	private:
    	FitnessCalculator * fitness;

		void solveProblem(const Description & description,
      					const Parameters & parameters, Preferences & preferences,
          				Solver & solver);

		void writeStartingPointsFile(string, ModelTuningParameters&);
    	void writeBoundsFile(string, ModelTuningParameters&);
    	void writeParametersFile(string);
    	void writeDescriptionFile(string);
    	void writePreferencesFile(string);

};

#endif
