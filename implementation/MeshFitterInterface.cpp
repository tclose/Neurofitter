#include "../MeshFitterInterface.h"

FitterResults MeshFitterInterface::runFitter(ModelTuningParameters * resolution, int seed) {

	ModelTuningParameters currentPoint(resolution->getLength());

	calculateMesh(0,*resolution,currentPoint);

	return FitterResults();
}

void MeshFitterInterface::calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint) {

	if (startDimension == resolution.getLength()) {
		fitness->calculateFitness(currentPoint);
	}
	else {
		currentPoint[startDimension] = resolution.getLowerBound(startDimension);  
		double dx = (resolution.getUpperBound(startDimension)-resolution.getLowerBound(startDimension))/resolution[startDimension];

		//"Smaller than or equal" to include upperbound]
		for (int i = 0; i <= resolution[startDimension]; i++) {
			calculateMesh(startDimension+1,resolution,currentPoint);
			currentPoint[startDimension] += dx; 
    	}
	}
}
