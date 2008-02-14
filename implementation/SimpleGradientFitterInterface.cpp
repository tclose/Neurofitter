/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include "../SimpleGradientFitterInterface.h"

FitterResults SimpleGradientFitterInterface::runFitter(ModelTuningParameters * startPoint) {

	vector< ModelTuningParameters > newPoints;
	vector< ModelTuningParameters > results;
	ModelTuningParameters bestPoint;

	bestPoint = startPoint[0];

	bool bestPointChanged = true;

	newPoints.push_back(bestPoint);
	errorValue->calculateParallelErrorValue(newPoints);
	bestPoint = newPoints[0];
	
	double numberOfEvaluations = 1;

	results.insert(results.end(),newPoints.begin(),newPoints.end());
	
	while (bestPointChanged && numberOfEvaluations < toInt(fixedParams["MaxNumberOfEvaluations"])) {

		newPoints.clear();

		generateSurrounding(bestPoint, toDouble(fixedParams["AddedValue"]), newPoints);
		
		errorValue->calculateParallelErrorValue(newPoints);
		numberOfEvaluations += newPoints.size();
		results.insert(results.end(),newPoints.begin(),newPoints.end());

		bool bestPointChanged = false;
		for (unsigned int i = 0; i < newPoints.size(); i++) {
			if (newPoints[i].getErrorValue() <= bestPoint.getErrorValue()) {
				bestPoint = newPoints[i];
				bestPointChanged = true;
			}
    	}
	}

	return FitterResults(results);
	
}

void SimpleGradientFitterInterface::generateSurrounding(ModelTuningParameters center, double addedValue, vector< ModelTuningParameters > & newPoints) {

	double newValue;
	for (int j = 0; j < center.getLength(); j++) {
		ModelTuningParameters newPointLeft(center);
		ModelTuningParameters newPointRight(center);
		newPointLeft.resetErrorValue();
		newPointRight.resetErrorValue();
		if ((newValue = center[j] + center[j]*addedValue) < center.getUpperBound(j)) {
			newPointRight[j] = newValue;
			newPoints.push_back(newPointRight);
		}
		if ((newValue = center[j] - center[j]*addedValue) > center.getLowerBound(j)) {
			newPointLeft[j] = newValue;
			newPoints.push_back(newPointLeft);
		}
	}

}
