/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_EOFITTERINTERFACE_H
#define NEUROFITTER_EOFITTERINTERFACE_H

//#include <eo>

#include <fstream>
#include <es/make_es.h>

// ParadisEO
//#include <eoDistPopLoopEval.h>
//#include <eoDistPopLoopSolver.h>


#include "FitterInterface.h"
#include "ErrorValueCalculator.h"
#include "EOErrorValueCalculator.h"
#include "EODistFitness.h"
#include "FixedParamObject.h"

class eoParser;
class eoState;

class EOFitterInterface : public FitterInterface, public FixedParamObject {

	public:
		EOFitterInterface(ErrorValueCalculator * fit, FixedParameters params):FitterInterface(fit),FixedParamObject(params){};
		/////
		//Inherited from FitterInterface
		/////
		virtual FitterResults runFitter(ModelTuningParameters * startPoints);

	private:
		void writeEOParamsFile(string fileName, ModelTuningParameters & start);
		template <class EOT> EOT runAlgorithm(EOT, eoParser& _parser, eoState& _state);

};

#endif
