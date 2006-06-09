#ifndef NORMALNEUROFITTERSTARTER_H
#define NORMALNEUROFITTERSTARTER_H

///todo include the right header files
#include "Neurofitter.h"
#include "NeurofitterStarter.h"

class NormalNeurofitterStarter : public NeurofitterStarter, FixedParamObject {

public:
	NormalNeurofitterStarter(FixedParameters params);
	void run(int argc, char** argv);

};

#endif
