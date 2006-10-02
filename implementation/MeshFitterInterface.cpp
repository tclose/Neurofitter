#include "../MeshFitterInterface.h"

FitterResults MeshFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	ModelTuningParameters resolution(fixedParams["MeshSize"],toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

	showMessage("Creating MeshInterface with " + fixedParams["Dimensions"] + " dimensions\n",5,fixedParams);

	vector< ModelTuningParameters > mesh;


	ModelTuningParameters currentPoint("",toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

	calculateMesh(0,resolution,currentPoint, mesh);

	fitness->calculateParallelFitness(mesh);

	return FitterResults(mesh);
}

void MeshFitterInterface::calculateMesh(int startDimension, ModelTuningParameters & resolution, ModelTuningParameters & currentPoint, vector< ModelTuningParameters > & mesh ) {

	if (startDimension == toInt(fixedParams["MeshDimensions"])) {
		mesh.push_back(currentPoint);
	}
	else {
		currentPoint[startDimension] = resolution.getLowerBound(startDimension);  
		double dx = (resolution.getUpperBound(startDimension)-resolution.getLowerBound(startDimension))/(resolution[startDimension]-1);

		//"Smaller than or equal" to include upperbound
		for (int i = 0; i <= resolution[startDimension]-1; i++) {
			calculateMesh(startDimension+1,resolution,currentPoint,mesh);
			currentPoint[startDimension] += dx; 
    	}
	}
}
