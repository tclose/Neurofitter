/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FileFitterInterface.h"

FitterResults FileFitterInterface::runFitter(ModelTuningParameters * unusedStartPoint) {

	showMessage("Creating FileFitterInterface with " + fixedParams["Dimensions"] + " dimensions\n",5,fixedParams);

	ModelTuningParameters initPoint("",toInt(fixedParams["Dimensions"]),fixedParams["Bounds"]);

	///The vector to store the points read from the file
	vector< ModelTuningParameters > paramList(toInt(fixedParams["NumberOfPoints"]), initPoint);

	ifstream paramFile((fixedParams["Filename"]).c_str(), ios::in);
    if (!paramFile.good()) crash("FileFitterInterface","Error while opening file "+fixedParams["Filename"]);
    showMessage("Reading from file: " + fixedParams["Filename"] + "\n",5,fixedParams);


	///Read the points
	for (int i = 0; i < toInt(fixedParams["NumberOfPoints"]); i++) {
		for (int j = 0; j < toInt(fixedParams["Dimensions"]); j++) {
			if (!paramFile.good()) crash("FileFitterInterface","Error while reading file "+fixedParams["Filename"]);
			paramFile >> (paramList[i][j]) ;
		}
	}

	///Evaluate all the parameters from the file in parallel
	errorValue->calculateParallelErrorValue(paramList);

	return FitterResults(paramList);
}

