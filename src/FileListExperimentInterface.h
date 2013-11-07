/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_FILELISTEXPERIMENTINTERFACE_H
#define NEUROFITTER_FILELISTEXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "TracesReader.h"
#include "ModelResults.h"
#include "FixedParamObject.h"

using namespace std;

class FileListExperimentInterface : public ExperimentInterface, public FixedParamObject {

public:
	FileListExperimentInterface(TracesReader * tracesReader, FixedParameters);
	virtual ModelResults getData() const;
	~FileListExperimentInterface() {};

private:
	TracesReader * tracesReader;
};

#endif
