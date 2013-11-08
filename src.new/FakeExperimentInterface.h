/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#ifndef NEUROFITTER_FAKEEXPERIMENTINTERFACE_H
#define NEUROFITTER_FAKEEXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "GenesisModelInterface.h"
#include "ModelResults.h"
#include "FixedParamObject.h"

using namespace std;

class FakeExperimentInterface : public ExperimentInterface, public FixedParamObject {

public:
	FakeExperimentInterface(ModelInterface*,FixedParameters);
	virtual ModelResults getData() const;
	~FakeExperimentInterface() {};

private:
	ModelInterface * model;

};

#endif
