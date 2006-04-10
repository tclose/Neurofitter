#ifndef EOFITTERINTERFACE_H
#define EOFITTERINTERFACE_H

#include <eo>

#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "EOFitnessCalculator.h"

class eoParser;
class eoState;

class EOFitterInterface : FitterInterface {

	public:
		//Inherited from FitterInterface
		virtual void runFitter(FitnessCalculator *, ModelTuningParameters * startPoints, int seed);

	private:
		void writeEOParamsFile(string fileName, ModelTuningParameters & start, int seed);
		template <class EOT> void runAlgorithm(EOT, eoParser& _parser, eoState& _state);

};

#endif
