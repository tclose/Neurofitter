/*
Revision of last commit: $Rev: 241 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-22 16:17:53 +0900 (Mon, 22 Oct 2007) $
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
