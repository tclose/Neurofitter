#ifndef EVOLUFIT_H
#define EVOLUFIT_H

///todo add EVOLUFIT to header file constants

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

//#include <eo>
//#include <es/make_es.h>

using namespace std;

#include "EvolufitState.h"
#include "EvolufitParameters.h"
#include "DataTrace.h"
#include "XMLString.h"
#include "FixedParameters.h"

#include "ModelTuningParameters.h"
#include "ModelResults.h"
#include "ExperimentInterface.h"
#include "ModelInterface.h"
#include "FitnessCalculator.h"
#include "FitterInterface.h"

#include "PabloModelInterface.h"
#include "WernerModelInterface.h"
#include "PabloExperimentInterface.h"
#include "WernerExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "PabloFitnessCalculator.h"

#include "EOFitnessCalculator.h"

#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "PabloFitterInterface.h"
#include "MeshFitterInterface.h"
#include "SwarmFitterInterface.h"

#endif
