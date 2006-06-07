#ifndef RANDOMFITTERINTERFACE_H
#define RANDOMFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"
#include "MersenneTwister.h"

using namespace std;

///todo make it possible to make a slice in a certain dimension
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

