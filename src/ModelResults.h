/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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
