#ifndef WERNERMODELINTERFACE_H
#define WERNERMODELINTERFACE_H

#include <fstream>

using namespace std;

#include "ModelInterface.h"

class WernerModelInterface : public ModelInterface {

public:	
    virtual ModelResults runModel(const ModelTuningParameters &) const;
	~WernerModelInterface() {};

	static void readDataFromFile(ModelResults&, string inputFileName, int startTrace, int numberOfRecordSites, int numberOfPeriods, double injWeight);

private:

};

#endif
