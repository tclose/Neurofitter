#ifndef WERNEREXPERIMENTINTERFACE_H
#define WERNEREXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "FixedParameters.h"
#include "WernerModelInterface.h"
#include "ModelResults.h"

class WernerExperimentInterface : public ExperimentInterface {

public:
	WernerExperimentInterface(FixedParameters);
	virtual ModelResults getData() const;
	~WernerExperimentInterface() {};

private:
	FixedParameters fixedParams;
};

#endif
