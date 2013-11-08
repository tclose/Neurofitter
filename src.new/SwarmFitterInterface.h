/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
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
