/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
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
	void setNewPositionFitness(ModelTuningParameters & newPosition);

	double getFitnessValue();

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
