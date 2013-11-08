/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
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
			paramFile >> (paramList[i][j]) ;
			if (!paramFile.good()) crash("FileFitterInterface","Error while reading file "+fixedParams["Filename"]);
		}
	}

	///Evaluate all the parameters from the file in parallel
	errorValue->calculateParallelErrorValue(paramList);

	return FitterResults(paramList);
}

