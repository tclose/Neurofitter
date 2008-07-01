/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_SIMPLEGRADIENTFITTERINTERFACE_H
#define NEUROFITTER_SIMPLEGRADIENTFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

using namespace std;

class SimpleGradientFitterInterface : public FitterInterface, FixedParamObject {

    public:
        SimpleGradientFitterInterface(ErrorValueCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution);

	private:
		void generateSurrounding(ModelTuningParameters center, double addedValue, vector< ModelTuningParameters > & newPoints);

};

#endif

