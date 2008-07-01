/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_NSGA2FITTERINTERFACE_H
#define NEUROFITTER_NSGA2FITTERINTERFACE_H

#include <vector>

#include "MersenneTwister.h"

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "NSGA2Individual.h"
#include "NSGA2Population.h"
#include "FixedParamObject.h"

using namespace std;

class NSGA2Individual;
class NSGA2Population;

class NSGA2FitterInterface : public FitterInterface, FixedParamObject {

    public:
        NSGA2FitterInterface(ErrorValueCalculator * fit, FixedParameters params);
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * startPoints);

	private:
		vector< NSGA2Individual * > sortFront(vector< NSGA2Individual * > front) const;

		NSGA2Population parents;
		NSGA2Population population;
		NSGA2Population children;
};

#endif
