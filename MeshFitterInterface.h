#ifndef NEUROFITTER_MESHFITTERINTERFACE_H
#define NEUROFITTER_MESHFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "FixedParamObject.h"

using namespace std;

///todo make it possible to make a slice in a certain dimension
class MeshFitterInterface : public FitterInterface, FixedParamObject {

    public:
        MeshFitterInterface(FitnessCalculator * fit, FixedParameters params):FitterInterface(fit), FixedParamObject(params) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution);

	private:
		void calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint, vector< ModelTuningParameters > & mesh);

};

#endif

