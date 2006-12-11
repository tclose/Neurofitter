/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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
