#ifndef MESHFITTERINTERFACE_H
#define MESHFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

class MeshFitterInterface : FitterInterface, FixedParamObject {

    public:
        MeshFitterInterface(FitnessCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution, int seed);

	private:
		void calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint);

};

#endif

