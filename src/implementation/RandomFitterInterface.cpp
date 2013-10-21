/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#include "../MersenneTwister.h"

#include "../RandomFitterInterface.h"

FitterResults RandomFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	showMessage("Creating RandomInterface with " + fixedParams["Dimensions"] + " dimensions\n",5,fixedParams);

	ModelTuningParameters initPoint("",toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

	///The vector to store the random points
	vector< ModelTuningParameters > randomList(toInt(fixedParams["NumberOfPoints"]), initPoint);

	///A random number generator
	MTRand randGen( toInt(fixedParams["Seed"]) );

	///Generate the random points
	for (int i = 0; i < toInt(fixedParams["NumberOfPoints"]); i++) {
		for (int j = 0; j < toInt(fixedParams["Dimensions"]); j++) {
			randomList[i][j] = randomList[i].getLowerBound(j)+randGen.rand(randomList[i].getUpperBound(j)-randomList[i].getLowerBound(j));
		}
	}

	///Evaluate all the random in parallel
	errorValue->calculateParallelErrorValue(randomList);

	return FitterResults(randomList);
}

