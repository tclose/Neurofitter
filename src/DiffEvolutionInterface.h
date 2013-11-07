/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_DIFFEVOLUTIONFITTERINTERFACE_H
#define NEUROFITTER_DIFFEVOLUTIONFITTERINTERFACE_H

#include <vector>

#include "MersenneTwister.h"

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

using namespace std;

class DiffEvolutionFitterInterface : public FitterInterface, FixedParamObject {

    public:
        DiffEvolutionFitterInterface(ErrorValueCalculator * fit, FixedParameters params)
			: FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * startPoints);

	private:
		int numberOfIndividuals;
		int numberOfGenerations;
};

#endif
