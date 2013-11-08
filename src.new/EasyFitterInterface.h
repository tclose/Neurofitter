/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_EASYFITTERINTERFACE_H
#define NEUROFITTER_EASYFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"
#include "MersenneTwister.h"

using namespace std;

class EasyFitterInterface : public FitterInterface, FixedParamObject {

    public:
        EasyFitterInterface(ErrorValueCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution);

	private:
		void generateSurrounding(ModelTuningParameters center, ModelTuningParameters radii, MTRand & randGen, vector< ModelTuningParameters > & newPoints);

};

#endif

