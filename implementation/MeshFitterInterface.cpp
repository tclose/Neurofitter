#include "../MeshFitterInterface.h"

FitterResults MeshFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint, int unusedSeed) {

	ModelTuningParameters resolution(fixedParams["MeshSize"],toInt(fixedParams["MeshDimensions"]),fixedParams["Bounds"]);

	if (toInt(fixedParams["MeshSize"]) > 4) {
		cout << "Creating MeshInterface with " << toInt(fixedParams["MeshDimensions"]) << " dimensions" << endl;
	}

	ModelTuningParameters currentPoint("",toInt(fixedParams["MeshDimensions"]),fixedParams["Bounds"]);

	calculateMesh(0,resolution,currentPoint);

	return FitterResults();
}

void MeshFitterInterface::calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint) {

	if (startDimension == toInt(fixedParams["MeshDimensions"])) {
		fitness->calculateFitness(currentPoint);
	}
	else {
		currentPoint[startDimension] = resolution.getLowerBound(startDimension);  
		double dx = (resolution.getUpperBound(startDimension)-resolution.getLowerBound(startDimension))/resolution[startDimension];

		//"Smaller than or equal" to include upperbound
		for (int i = 0; i <= resolution[startDimension]; i++) {
			calculateMesh(startDimension+1,resolution,currentPoint);
			currentPoint[startDimension] += dx; 
    	}
	}
}
