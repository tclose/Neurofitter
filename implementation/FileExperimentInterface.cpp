#include "../FileExperimentInterface.h"

FileExperimentInterface::FileExperimentInterface(TracesReader * t, FixedParameters params)
	: FixedParamObject(params), tracesReader(t) {}

ModelResults FileExperimentInterface::getData() const{

	return tracesReader->readTraces(fixedParams["FilesDirectory"]);

}
