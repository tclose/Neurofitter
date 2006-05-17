#ifndef MODELRESULTS_H
#define MODELRESULTS_H

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
