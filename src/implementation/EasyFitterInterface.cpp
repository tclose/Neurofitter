/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../EasyFitterInterface.h"

FitterResults EasyFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	showMessage("Creating Easy Fitter with " + fixedParams["NumberOfThreads"] + " threads and " + fixedParams["NumberOfTriesPT"] + " points per thread\n",5,fixedParams);

	double sigma = toDouble(fixedParams["Sigma"]);

	///A random number generator
    MTRand randGen( toInt(fixedParams["Seed"]) );

	ModelTuningParameters initPoint("",toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);
	vector< ModelTuningParameters > newPoints;
	vector< ModelTuningParameters > results;

    vector< ModelTuningParameters > bestPoints(toInt(fixedParams["NumberOfThreads"]), initPoint);
    for (unsigned i = 0; i < toUnsigned(fixedParams["NumberOfThreads"]); i++) {
		for (unsigned j = 0; j < bestPoints[i].getLength(); j++) {
			bestPoints[i][j] = bestPoints[i].getLowerBound(j)+randGen.rand(bestPoints[i].getUpperBound(j)-bestPoints[i].getLowerBound(j));
		}
	}
    
	errorValue->calculateParallelErrorValue(bestPoints);
	
	results.insert(results.end(),bestPoints.begin(),bestPoints.end());

	vector< ModelTuningParameters > radii(toInt(fixedParams["NumberOfThreads"]), initPoint);
    for (unsigned i = 0; i < toUnsigned(fixedParams["NumberOfThreads"]); i++) {
		for (unsigned j = 0; j < radii[i].getLength(); j++) {
			radii[i][j] = randGen.rand((radii[i].getUpperBound(j)-radii[i].getLowerBound(j))/5);
		}
	}		
	
    for (int i = 0; i < toInt(fixedParams["NumberOfRuns"]); i++) {

		showMessage("\nStarting Run " + str(i) + "\n",2,fixedParams);

		newPoints.clear();

		///Generate the new points
    	for (int j = 0; j < toInt(fixedParams["NumberOfThreads"]); j++) {
			generateSurrounding(bestPoints[j], radii[j], randGen, newPoints);		
    	}
		errorValue->calculateParallelErrorValue(newPoints);
		results.insert(results.end(),newPoints.begin(),newPoints.end());

    	for (int j = 0; j < toInt(fixedParams["NumberOfThreads"]); j++) {
			bool bestPointChanged = false;
			for (int k = j*toInt(fixedParams["NumberOfTriesPT"]); k < (j+1)*toInt(fixedParams["NumberOfTriesPT"]); k++) {
				if (newPoints[k].getErrorValue() < bestPoints[j].getErrorValue()) {
					for (unsigned s = 0; s < radii[j].getLength(); s++) { 
						if (bestPoints[j][s] != newPoints[k][s]) {
							radii[j][s] = fabs(bestPoints[j][s] - newPoints[k][s])/2.0;
							showMessage("Changed radius in dimension " + str(s) + " of thread " + str(j) + " to " + str(radii[j][s]) + "\n",4,fixedParams);
						}
					}
					bestPoints[j] = newPoints[k];
					bestPointChanged = true;
				}
			}
			if (!bestPointChanged) {
				for (unsigned s = 0; s < radii[j].getLength(); s++) { 
					radii[j][s] = min(sigma*radii[j][s], radii[j].getUpperBound(s)-radii[j].getLowerBound(s));
					showMessage("Increased radius in dimension " + str(s) + " of thread " + str(j) + " to " + str(radii[j][s]) + "\n",4,fixedParams);
				}
			} 
    	}		
	}

	return FitterResults(results);
	
}

void EasyFitterInterface::generateSurrounding(ModelTuningParameters center, ModelTuningParameters radii, MTRand & randGen, vector< ModelTuningParameters > & newPoints) {

	double sigma = toDouble(fixedParams["Sigma"]);

	ModelTuningParameters newPoint(center);
	newPoint.resetErrorValue();

	double newValue;
	for (unsigned i = 0; i < toUnsigned(fixedParams["NumberOfTriesPT"]); i++) {
		for (unsigned j = 0; j < center.getLength(); j++) {
			if (randGen.rand(1) < 0.5) 
				newValue = center[j] + randGen.rand(sigma*radii[j]);
				//newValue = center[j] + randGen.randNorm(0,radii[j]);
			else
				newValue = center[j] - randGen.rand(sigma*radii[j]);
				//newValue = center[j] - randGen.randNorm(0,radii[j]);
			//if (newValue > center.getUpperBound(j)) newValue = center.getUpperBound(j);
			//if (newValue < center.getLowerBound(j)) newValue = center.getLowerBound(j);
			if (newValue > center.getUpperBound(j) || newValue < center.getLowerBound(j)) {
				newValue = center.getLowerBound(j)+randGen.rand(center.getUpperBound(j)-center.getLowerBound(j));
			}
        	newPoint[j] = newValue;
		}
		newPoints.push_back(newPoint);
	}

}
