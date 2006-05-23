#ifndef EVOLUFIT_H
#define EVOLUFIT_H

///todo add EVOLUFIT to header file constants

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "XMLString.h"
#include "FixedParameters.h"

#include "ModelTuningParameters.h"
#include "ModelResults.h"
#include "ExperimentInterface.h"
#include "ModelInterface.h"
#include "FitnessCalculator.h"
#include "FitterInterface.h"

#include "GenesisModelInterface.h"
#include "FakeExperimentInterface.h"
#include "PabloVdVdtMatrix.h"
#include "PabloFitnessCalculator.h"

#include "EOFitnessCalculator.h"

#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "PabloFitterInterface.h"
#include "MeshFitterInterface.h"
#include "SwarmFitterInterface.h"

#endif
