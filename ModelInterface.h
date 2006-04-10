#ifndef MODELINTERFACE_H
#define MODELINTERFACE_H

#include "ModelResults.h"
#include "ModelTuningParameters.h"

class ModelInterface {

public:

	virtual ModelResults runModel(const ModelTuningParameters &) const = 0;
	virtual ~ModelInterface() {};

private:	


};

#endif
