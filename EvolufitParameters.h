#ifndef EVOLUFIT_PARAMETERS_H
#define EVOLUFIT_PARAMETERS_H

#include <string>
#include <iostream>

#include "EvolufitState.h"

using namespace std;

class EvolufitParameters {

public:
	EvolufitParameters(EvolufitState *);
	void readFromFile(string filename);
	void writeToFile(string filename);
	int getVerboseLevel() const;
	string getDataFileLoc() const;

private:
	int verboseLevel;
	string dataFileLoc;	
	EvolufitState * state;
	
};

#endif
