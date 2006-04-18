#include "../SwarmFitterInterface.h"
#include "../MersenneTwister.h"

FitterResults SwarmFitterInterface::runFitter(ModelTuningParameters * startPoint, int seed) {

	SwarmFly * swarm = new SwarmFly [numberOfFlies];
	//SwarmFly::bestGlobalFitnessValue = 10000000;

	ModelTuningParameters startP(startPoint->getLength());
	ModelTuningParameters startSpeed(startPoint->getLength());
	
	MTRand randGen( (unsigned)time( NULL ) );

	for (int i = 0; i < numberOfFlies; i++) {
		for (int j = 0; j < startP.getLength(); j++) {
			startP[j] = startPoint->getLowerBound(j)+(startPoint->getUpperBound(j)-startPoint->getLowerBound(j))*randGen.rand();
		}
		for (int j = 0; j < startSpeed.getLength(); j++) {
			startSpeed[j] = 0.0*randGen.rand();
		}
		swarm[i].setMembers(fitness,0.9,0.9,startP,startSpeed);
	}

	for (int i = 0; i < numberOfRuns; i++) {
		for (int j = 0; j < numberOfFlies; j++) {
			swarm[j].fly();
		}
		cout << "Best solution after run " << i << " : " << SwarmFly::bestGlobalSolution.toString() << " : " << SwarmFly::bestGlobalFitnessValue << endl;
	}

	return FitterResults();
}

