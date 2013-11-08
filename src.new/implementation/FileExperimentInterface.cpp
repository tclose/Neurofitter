/*
Revision of last commit: $Rev: 241 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-22 16:17:53 +0900 (Mon, 22 Oct 2007) $
*/

#include "../FileExperimentInterface.h"

FileExperimentInterface::FileExperimentInterface(TracesReader * t, FixedParameters params)
	: FixedParamObject(params), tracesReader(t) {}

ModelResults FileExperimentInterface::getData() const{

	return tracesReader->readTracesFromDirectory(fixedParams["FilesDirectory"]);

}
