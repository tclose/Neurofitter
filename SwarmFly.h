#ifndef SWARMFLY_H
#define SWARMFLY_H

#include "SwarmFitterInterface.h"


class SwarmFly {
	
public:
	SwarmFly() : fitness(NULL), c1(-1), c2(-1), bestLocalFitnessValue(10000000) {} ///todo change MAX constant 

	void setMembers(FitnessCalculator *fit, double newC1, double newC2, ModelTuningParameters startPoint, ModelTuningParameters startSpeed);
	void fly();
	void move(ModelTuningParameters position);


	FitnessCalculator *fitness;
	ModelTuningParameters currentPosition;
	ModelTuningParameters currentSpeed;
	ModelTuningParameters bestLocalSolution;

	double c1, c2;	

	double bestLocalFitnessValue;

	static ModelTuningParameters bestGlobalSolution;
	static double bestGlobalFitnessValue;
	static double inertial;
};

#endif