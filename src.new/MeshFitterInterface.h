/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_MESHFITTERINTERFACE_H
#define NEUROFITTER_MESHFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "FixedParamObject.h"

using namespace std;

class MeshFitterInterface : public FitterInterface, FixedParamObject {

    public:
        MeshFitterInterface(ErrorValueCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution);

	private:
		void calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint, vector< ModelTuningParameters > & mesh);

};

#endif

