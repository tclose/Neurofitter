/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
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
