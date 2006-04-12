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
		EOFitterInterface(FitnessCalculator * fit):FitterInterface(fit){};
		/////
		//Inherited from FitterInterface
		/////
		virtual FitterResults runFitter(ModelTuningParameters * startPoints, int seed);

	private:
		void writeEOParamsFile(string fileName, ModelTuningParameters & start, int seed);
		template <class EOT> EOT runAlgorithm(EOT, eoParser& _parser, eoState& _state);

};

#endif
