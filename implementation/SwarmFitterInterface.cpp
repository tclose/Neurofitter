#include "../SwarmFitterInterface.h"

// Standard PSO version 2006

FitterResults SwarmFitterInterface::runFitter(ModelTuningParameters * startPoint) {

	int numberOfFlies = (int)(10.0+2.0*sqrt(toInt(fixedParams["Dimensions"])));
	double w = 1.0/(2.0*log(2.0));
	double c = 0.5 + log(2.0);
	
	(void)w;
	(void)c;

	int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);
	if (numberOfRuns < 0) crash("SwarmFitterInterface","Negative number of runs !!");

	MTRand randGen( toInt(fixedParams["Seed"]) );

	vector< SwarmFly > swarm(numberOfFlies, SwarmFly(&randGen));

	ModelTuningParameters startP(startPoint->getLength());
	ModelTuningParameters startX;
	ModelTuningParameters startY;
	ModelTuningParameters startSpeed(startPoint->getLength());
	
	for (int i = 0; i < numberOfFlies; i++) {
		///todo check random bounds
		for (int j = 0; j < startP.getLength(); j++) {
			startP[j] = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
		}
		for (int j = 0; j < startSpeed.getLength(); j++) {
			double startX = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
			double startY = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
			startSpeed[j] = startX-startY;
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

