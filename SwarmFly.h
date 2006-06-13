#ifndef SWARMFLY_H
#define SWARMFLY_H

#include "MersenneTwister.h"

#include "FixedParamObject.h"
#include "SwarmFitterInterface.h"

using namespace std;

class SwarmFitterInterface;

class SwarmFly : public FixedParamObject {
	
public:
	SwarmFly(MTRand * rand, FixedParameters params) : FixedParamObject(params),c(-1), w(-1), randGen(rand), bestLocalInited(false) {};

	void setMembers(double w, double c, ModelTuningParameters startPoint, ModelTuningParameters startSpeed);

	ModelTuningParameters calculateNewPosition();
	void setNewPositionFitness(ModelTuningParameters & newPosition);

	void addInformant(SwarmFly *);
	vector< SwarmFly * > getInformants();
	void resetInformants();


	static ModelTuningParameters bestGlobalSolution;
	static bool bestGlobalInited;

private:
	ModelTuningParameters currentPosition;
	ModelTuningParameters currentSpeed;
	ModelTuningParameters bestLocalSolution;
	ModelTuningParameters bestInformantsSolution;

	double c, w;	

	vector< SwarmFly * > informants;

	MTRand * randGen;

	ModelTuningParameters getBestLocalSolution();

	bool bestLocalInited;

	void keepInBox(ModelTuningParameters & newPosition);

};

#endif
