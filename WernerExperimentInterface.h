#ifndef WERNEREXPERIMENTINTERFACE_H
#define WERNEREXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "WernerModelInterface.h"
#include "ModelResults.h"
#include "FixedParamObject.h"

class WernerExperimentInterface : public ExperimentInterface, public FixedParamObject {

public:
	WernerExperimentInterface(FixedParameters);
	virtual ModelResults getData() const;
	~WernerExperimentInterface() {};

private:
	//FixedParameters fixedParams;
};

#endif
