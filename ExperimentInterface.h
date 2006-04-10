#ifndef EXPERIMENTINTERFACE_H
#define EXPERIMENTINTERFACE_H

#include "ModelResults.h"

class ExperimentInterface {

public:
	virtual ModelResults getData() const = 0;
	virtual ~ExperimentInterface() {};

};

#endif
