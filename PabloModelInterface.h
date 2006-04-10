#ifndef PABLOMODELINTERFACE_H
#define PABLOMODELINTERFACE_H

#include <string>
#include <fstream>

using namespace std;

#include "ModelResults.h"
#include "ModelTuningParameters.h"
#include "ModelInterface.h"

class PabloModelInterface : public ModelInterface {

public:	
    virtual ModelResults runModel(const ModelTuningParameters &);
	~PabloModelInterface() {};

	static void readDataFromFile(ModelResults&, string inputFileName, int startTrace, int numberOfRecordSites, int numberOfPeriods);

private:



};

#endif
