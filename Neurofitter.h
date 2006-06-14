#ifndef NEUROFITTER_H
#define NEUROFITTER_H

///todo add NEUROFITTER to header file constants

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "XMLString.h"
#include "FixedParameters.h"

#include "TracesReader.h"
#include "NormalTracesReader.h"

#include "ModelTuningParameters.h"
#include "ModelResults.h"
#include "ExperimentInterface.h"
#include "ModelInterface.h"
#include "FitnessCalculator.h"
#include "FitterInterface.h"

#include "GenesisModelInterface.h"
#include "FakeExperimentInterface.h"
#include "FileExperimentInterface.h"
#include "NormalVdVdtMatrix.h"
#include "MatrixFitnessCalculator.h"

#include "EOFitnessCalculator.h"

#include "EOFitterInterface.h"
#include "NOMADFitterInterface.h"
#include "EONOMADFitterInterface.h"
#include "MeshFitterInterface.h"
#include "RandomFitterInterface.h"
#include "SwarmFitterInterface.h"

#endif
