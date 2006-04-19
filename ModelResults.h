#ifndef MODELRESULTS_H
#define MODELRESULTS_H

#include <iostream>

using namespace std;

#include "DataTrace.h"

///todo use vectors to implement this
class ModelResults {

public:
	ModelResults() {traces = NULL; numberOfTraces = 0;}
	ModelResults(const int numberOfTraces);
	ModelResults(const ModelResults & m) {*this = m;};

	ModelResults& operator=(const ModelResults &);

	~ModelResults();

	int getLength() const;
	DataTrace &operator[] (const int);
	const DataTrace &operator[] (const int) const;

private:
	DataTrace * traces;
	int numberOfTraces;

};

#endif
