/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EXPERIMENTINTERFACE_H
#define NEUROFITTER_EXPERIMENTINTERFACE_H

#include "ModelResults.h"

using namespace std;

class ExperimentInterface {

public:
	virtual ModelResults getData() const = 0;
	virtual ~ExperimentInterface() {};

};

#endif
