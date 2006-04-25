#ifndef FAKEEXPERIMENTINTERFACE_H
#define FAKEEXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "GenesisModelInterface.h"
#include "ModelResults.h"
#include "FixedParamObject.h"

class FakeExperimentInterface : public ExperimentInterface, public FixedParamObject {

public:
	FakeExperimentInterface(ModelInterface*,FixedParameters);
	virtual ModelResults getData() const;
	~FakeExperimentInterface() {};

private:
	ModelInterface * model;

};

#endif
