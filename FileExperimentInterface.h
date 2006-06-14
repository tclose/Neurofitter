#ifndef NEUROFITTER_FILEEXPERIMENTINTERFACE_H
#define NEUROFITTER_FILEEXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "TracesReader.h"
#include "ModelResults.h"
#include "FixedParamObject.h"

using namespace std;

class FileExperimentInterface : public ExperimentInterface, public FixedParamObject {

public:
	FileExperimentInterface(TracesReader * tracesReader, FixedParameters);
	virtual ModelResults getData() const;
	~FileExperimentInterface() {};

private:
	TracesReader * tracesReader;
};

#endif
