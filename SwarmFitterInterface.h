#ifndef SWARMFITTERINTERFACE_H
#define SWARMFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "SwarmFly.h"

class SwarmFitterInterface : FitterInterface {

    public:
        SwarmFitterInterface(FitnessCalculator * fit, int numberOfFlies, int numberOfRuns):FitterInterface(fit), numberOfFlies(numberOfFlies), numberOfRuns(numberOfRuns) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * startPoints, int seed);

	private:
		int numberOfFlies;
		int numberOfRuns;
};

#endif

