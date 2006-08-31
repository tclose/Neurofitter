#include "../RandomFitterInterface.h"

FitterResults RandomFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	if (toInt(fixedParams["VerboseLevel"]) > 4) {
		cout << "Creating RandomInterface with " << toInt(fixedParams["Dimensions"]) << " dimensions" << endl;
	}

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
	fitness->calculateParallelFitness(randomList);

	return FitterResults(randomList);
}

