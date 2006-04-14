#ifndef MESHFITTERINTERFACE_H
#define MESHFITTERINTERFACE_H

#include "FitterResults.h"
#include "FitterInterface.h"
#include "FitnessCalculator.h"

class MeshFitterInterface : FitterInterface {

    public:
        MeshFitterInterface(FitnessCalculator * fit):FitterInterface(fit) {};
        /////
        //Inherited from FitterInterface
        /////
        virtual FitterResults runFitter(ModelTuningParameters * resolution, int seed);

	private:
		void calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint);

};

#endif

