/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_RANDOMFITTERINTERFACE_H
#define NEUROFITTER_RANDOMFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

using namespace std;

class RandomFitterInterface : public FitterInterface, FixedParamObject {

    public:
        RandomFitterInterface(FitnessCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters *);

	private:

};

#endif

