#ifndef MPIEVOLUFITSTARTER_H
#define MPIEVOLUFITSTARTER_H

///todo include the right header files
#include "Evolufit.h"
#include "EvolufitStarter.h"

#include "MPIModelInterface.h"

class MPIEvolufitStarter : public EvolufitStarter, FixedParamObject {

public:
	MPIEvolufitStarter(FixedParameters params);
	void run(int argc, char** argv);

};

#endif
