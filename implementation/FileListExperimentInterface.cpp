/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../FileListExperimentInterface.h"

FileListExperimentInterface::FileListExperimentInterface(TracesReader * t, FixedParameters params)
	: FixedParamObject(params), tracesReader(t) {}

ModelResults FileListExperimentInterface::getData() const{

	return tracesReader->readTracesFromFilesList(fixedParams["FilesList"]);

}
