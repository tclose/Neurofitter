/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FileExperimentInterface.h"

FileExperimentInterface::FileExperimentInterface(TracesReader * t, FixedParameters params)
	: FixedParamObject(params), tracesReader(t) {}

ModelResults FileExperimentInterface::getData() const{

	return tracesReader->readTracesFromDirectory(fixedParams["FilesDirectory"]);

}
