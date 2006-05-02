#ifndef EOFITTERINTERFACE_H
#define EOFITTERINTERFACE_H

#include <eo>

#include "FitterInterface.h"
#include "FitnessCalculator.h"
#include "EOFitnessCalculator.h"
#include "FixedParamObject.h"

class eoParser;
class eoState;

///todo check the "public" from FitterInterface

class EOFitterInterface : public FitterInterface, public FixedParamObject {

	public:
		EOFitterInterface(FitnessCalculator * fit, FixedParameters params):FitterInterface(fit),FixedParamObject(params){};
		/////
		//Inherited from FitterInterface
		/////
		virtual FitterResults runFitter(ModelTuningParameters * startPoints);

	private:
		void writeEOParamsFile(string fileName, ModelTuningParameters & start);
		template <class EOT> EOT runAlgorithm(EOT, eoParser& _parser, eoState& _state);

};

#endif
