#include "../RandomFitterInterface.h"

FitterResults RandomFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	if (toInt(fixedParams["VerboseLevel"]) > 4) {
		cout << "Creating RandomInterface with " << toInt(fixedParams["Dimensions"]) << " dimensions" << endl;
	}

	ModelTuningParameters initPoint("",toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

	vector< ModelTuningParameters > randomList(toInt(fixedParams["NumberOfPoints"]), initPoint);

	MTRand randGen( toInt(fixedParams["Seed"]) );

	for (int i = 0; i < toInt(fixedParams["NumberOfPoints"]); i++) {
		for (int j = 0; j < toInt(fixedParams["Dimensions"]); j++) {
			///todo Check the bounds and maybe make more efficient
			randomList[i][j] = randomList[i].getLowerBound(j)+randGen.rand(randomList[i].getUpperBound(j)-randomList[i].getLowerBound(j));
		}
	}

	fitness->calculateParallelFitness(randomList);

	return FitterResults();
}

