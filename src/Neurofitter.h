/*
Revision of last commit: $Rev$     
Author of last commit: $Author$  
Date of last commit: $Date$    
*/

#ifndef NEUROFITTER_H
#define NEUROFITTER_H

#ifdef WITH_MPI
	#include <mpi.h>
	#include "MPIModelInterface.h"
	#include "MPIErrorValueCalculator.h"
#endif

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
#include "ErrorValueCalculator.h"
#include "FitterInterface.h"

#include "GenesisModelInterface.h"
#include "NeuronModelInterface.h"
#include "ExecutableModelInterface.h"
#include "FakeExperimentInterface.h"
#include "FileExperimentInterface.h"
#include "FileListExperimentInterface.h"
#include "VdVdtMatrix.h"
#include "MeshVdVdtMatrix.h"
#include "MapVdVdtMatrix.h"
#include "DirectVdVdtMatrix.h"
#include "DistanceVdVdtMatrix.h"
#include "ROI.h"
#include "ROIStringParser.h"

#include "MatrixErrorValueCalculator.h"
#include "RodrigoErrorValueCalculator.h"
#include "RMSErrorValueCalculator.h"
#include "EasyFitterInterface.h"
#include "SimpleGradientFitterInterface.h"
#include "MeshFitterInterface.h"
#include "RandomFitterInterface.h"
#include "FileFitterInterface.h"
#include "SwarmFitterInterface.h"
#include "NSGA2FitterInterface.h"

#ifdef WITH_EO
	#include "EOErrorValueCalculator.h"
	#include "EOFitterInterface.h"
#endif

#ifdef WITH_NOMAD
	#include "NOMADFitterInterface.h"
#endif

#ifdef WITH_EO
	#ifdef WITH_NOMAD
		#include "EONOMADFitterInterface.h"
	#endif
#endif



#endif
