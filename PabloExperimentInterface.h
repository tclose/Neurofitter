#ifndef PABLOEXPERIMENTINTERFACE_H
#define PABLOEXPERIMENTINTERFACE_H

#include <string>

#include "ModelResults.h"
#include "ExperimentInterface.h"
#include "PabloModelInterface.h"
#include "Tools.h"

using namespace std;

class PabloExperimentInterface : public ExperimentInterface {

public:
	virtual ModelResults getData();
	~PabloExperimentInterface() {};

};

#endif
