#ifndef NORMALEVOLUFITSTARTER_H
#define NORMALEVOLUFITSTARTER_H

///todo include the right header files
#include "Evolufit.h"
#include "EvolufitStarter.h"

class NormalEvolufitStarter : public EvolufitStarter, FixedParamObject {

public:
	NormalEvolufitStarter(FixedParameters params);
	void run(int argc, char** argv);

};

#endif
