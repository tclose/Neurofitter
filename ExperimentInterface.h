#ifndef EXPERIMENTINTERFACE_H
#define EXPERIMENTINTERFACE_H

#include "ModelResults.h"

using namespace std;

class ExperimentInterface {

public:
	virtual ModelResults getData() const = 0;
	virtual ~ExperimentInterface() {};

};

#endif
