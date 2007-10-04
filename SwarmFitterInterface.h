/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_SWARMFITTERINTERFACE_H
#define NEUROFITTER_SWARMFITTERINTERFACE_H

#include <vector>

#include "MersenneTwister.h"

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "SwarmFly.h"
#include "FixedParamObject.h"

using namespace std;

class SwarmFly;

class SwarmFitterInterface : public FitterInterface, FixedParamObject {

    public:
        SwarmFitterInterface(ErrorValueCalculator * fit, FixedParameters params)
			: FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * startPoints);

	private:
		int numberOfFlies;
		int numberOfRuns;

		void randomizeTopology(vector< SwarmFly > & swarm, int maxInformed, MTRand & randGen);
		void randomizeWorst(vector< SwarmFly > & swarm);
};

#endif
