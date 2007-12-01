/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
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

