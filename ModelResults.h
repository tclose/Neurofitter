#ifndef MODELRESULTS_H
#define MODELRESULTS_H

#include <iostream>
#include <vector>

using namespace std;

#include "DataTrace.h"

class ModelResults {

public:
	ModelResults();
	ModelResults(const int numberOfTraces);

	int getLength() const;
	DataTrace &operator[] (const int);
	const DataTrace &operator[] (const int) const;

	void printOn(ostream &) const;
	void readFrom(istream &);

private:
	vector< DataTrace > traces;

};

#endif
