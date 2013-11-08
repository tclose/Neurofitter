/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

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
