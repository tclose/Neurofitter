/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_NOMADFITTERINTERFACE_H
#define NEUROFITTER_NOMADFITTERINTERFACE_H

#include <nomad>
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

using namespace std;

extern void setNOMADErrorValueCalculator(ErrorValueCalculator*, int);

class NOMADFitterInterface : public FitterInterface, FixedParamObject {

	public:
		NOMADFitterInterface(ErrorValueCalculator * fit, FixedParameters params);
		////
		//Inherited from FitterInterface
    	////
		virtual FitterResults runFitter(ModelTuningParameters * startPt);

	private:
    	ErrorValueCalculator * errorValue;

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
