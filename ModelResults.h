#ifndef MODELRESULTS_H
#define MODELRESULTS_H

#include <iostream>
#include <vector>

using namespace std;

#include "DataTrace.h"

///todo use vectors to implement this
class ModelResults {

public:
	ModelResults();
	ModelResults(const int numberOfTraces);

	int getLength() const;
	DataTrace &operator[] (const int);
	const DataTrace &operator[] (const int) const;

private:
	vector< DataTrace > traces;
	int numberOfTraces;

};

#endif
