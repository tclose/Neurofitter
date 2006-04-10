#ifndef WERNEREXPERIMENTINTERFACE_H
#define WERNEREXPERIMENTINTERFACE_H

#include "ExperimentInterface.h"
#include "WernerModelInterface.h"
#include "ModelResults.h"

class WernerExperimentInterface : public ExperimentInterface {

public:
	virtual ModelResults getData() const;
	~WernerExperimentInterface() {};

};

#endif
