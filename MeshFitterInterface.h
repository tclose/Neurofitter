/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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

