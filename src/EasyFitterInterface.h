/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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

