/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#ifndef NEUROFITTER_MODELRESULTS_H
#define NEUROFITTER_MODELRESULTS_H

#include <iostream>
#include <vector>

using namespace std;

#include "DataTrace.h"

#include "InputChannel.h"
#include "OutputChannel.h"


class ModelResults {

public:
	ModelResults();
	ModelResults(const int numberOfTraces);

	int getLength() const;
	DataTrace &operator[] (const int);
	const DataTrace &operator[] (const int) const;

	void printOn(OutputChannel &) const;
	void readFrom(InputChannel &);

private:
	vector< DataTrace > traces;

};

#endif
