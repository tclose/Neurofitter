#ifndef WERNEREXPERIMENTINTERFACE_H
#define WERNEREXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "WernerModelInterface.h"
#include "ModelResults.h"

class WernerExperimentInterface : public ExperimentInterface {

public:
	WernerExperimentInterface(ModelTuningParameters);
	virtual ModelResults getData() const;
	~WernerExperimentInterface() {};

private:
	ModelTuningParameters params;

};

#endif
