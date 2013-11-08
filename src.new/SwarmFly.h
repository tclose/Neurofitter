/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_SWARMFLY_H
#define NEUROFITTER_SWARMFLY_H

#include "MersenneTwister.h"

#include "FixedParamObject.h"
#include "SwarmFitterInterface.h"

using namespace std;

class SwarmFitterInterface;

class SwarmFly : public FixedParamObject {
	
public:
	SwarmFly(double newW, double newC, MTRand * rand, FixedParameters params);

	ModelTuningParameters calculateNewPosition();
	ModelTuningParameters calculateRandomPosition();
	void setNewPositionErrorValue(ModelTuningParameters & newPosition);

	double getErrorValue();

	void addInformant(SwarmFly *);
	vector< SwarmFly * > getInformants();
	void resetInformants();

	void makeNextPositionRandom();
	
	ModelTuningParameters & getParameters();

	static ModelTuningParameters bestGlobalSolution;
	static bool bestGlobalInited;

private:
	SwarmFly();

	ModelTuningParameters currentPosition;
	ModelTuningParameters currentSpeed;
	ModelTuningParameters bestLocalSolution;
	ModelTuningParameters bestInformantsSolution;

	double c, w;	

	vector< SwarmFly * > informants;

	MTRand * randGen;

	ModelTuningParameters getBestLocalSolution();

	bool bestLocalInited;
	bool nextPositionRandom;

	void keepInBox(ModelTuningParameters & newPosition);

};

#endif
