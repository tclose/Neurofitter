#include "../SwarmFitterInterface.h"

// Standard PSO version 2006, for algorithm see end of file

FitterResults SwarmFitterInterface::runFitter(ModelTuningParameters * startPoint) {

	int numberOfFlies = (int)(10.0+2.0*sqrt((double)toInt(fixedParams["Dimensions"])));

	if (toInt(fixedParams["VerboseLevel"]) > 2) cout << "Running Swarm Optimization with " << numberOfFlies << " flies" << endl; 

	int maxInformed = 3;
	double w = 1.0/(2.0*log(2.0));
	double c = 0.5 + log(2.0);
	
	double tempBestValue = 0;
	///True if tempBestValue is initialized
	bool initBest = false; 

	int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);
	if (numberOfRuns < 0) crash("SwarmFitterInterface","Negative number of runs !!");

	MTRand randGen( toInt(fixedParams["Seed"]) );

	vector< SwarmFly > swarm(numberOfFlies, SwarmFly(&randGen, FixedParameters(fixedParams.getGlobals(),true)));

	vector< ModelTuningParameters > startP(numberOfFlies,ModelTuningParameters(*startPoint));
	vector< ModelTuningParameters > startSpeed(numberOfFlies,ModelTuningParameters(*startPoint));

	ModelTuningParameters startX;
	ModelTuningParameters startY;

	//////////////////////////////////
	/// Initialize the swarm flies ///
	//////////////////////////////////
	for (int i = 0; i < numberOfFlies; i++) {
		for (int j = 0; j < startP[i].getLength(); j++) {
			startP[i][j] = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
		}
		for (int j = 0; j < startSpeed[i].getLength(); j++) {
			double startX = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
			double startY = startPoint->getLowerBound(j)+randGen.rand(startPoint->getUpperBound(j)-startPoint->getLowerBound(j));
			startSpeed[i][j] = startX-startY;
		}
	}
	
	fitness->calculateParallelFitness(startP);

	for (int i = 0; i < numberOfFlies; i++) {
		swarm[i].setMembers(w,c,startP[i],startSpeed[i]);
	}

	///////////////////////////
	/// Initialize topology ///
	///////////////////////////
	randomizeTopology(swarm, maxInformed, randGen);
	
	/////////////////////
	/// Run Algorithm ///
	/////////////////////
	vector< ModelTuningParameters > flyPositions(numberOfFlies);

	for (int i = 0; i < numberOfRuns; i++) {		
		for (int j = 0; j < numberOfFlies; j++) {
			flyPositions[j] = swarm[j].calculateNewPosition();
		}
		fitness->calculateParallelFitness(flyPositions);
		for (int j = 0; j < numberOfFlies; j++) {
			swarm[j].setNewPositionFitness(flyPositions[j]);
		}
		if (toInt(fixedParams["VerboseLevel"]) > 1)
			cout << "Best solution after run " << i << " : " << SwarmFly::bestGlobalSolution.toString() << " : " << SwarmFly::bestGlobalSolution.getFitnessValue() << endl;
		if (SwarmFly::bestGlobalSolution.getFitnessValue() < tempBestValue || !initBest) {
			tempBestValue = SwarmFly::bestGlobalSolution.getFitnessValue();
			initBest = true;
		}
		else {
			if (toInt(fixedParams["VerboseLevel"]) > 2) cout << "No better solution found in the last run: Randomizing swarm topology" << endl; 
			randomizeTopology(swarm, maxInformed, randGen);
		}

	}

	return FitterResults();
}

void SwarmFitterInterface::randomizeTopology(vector< SwarmFly > & swarm, int maxInformed, MTRand & randGen) {

	for (int i = 0; i < (int)swarm.size(); i++) {
		swarm[i].resetInformants();
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << i << " informs: { "; 
		for (int j = 0; j < maxInformed; j++) {
			int randI = randGen.randInt(swarm.size()-1);
			swarm[randI].addInformant(&(swarm[j]));
			if (toInt(fixedParams["VerboseLevel"]) > 3) cout << randI << " "; 
		}
		if (toInt(fixedParams["VerboseLevel"]) > 3) cout << "}" << endl; 
	}

}


/* Standard PSO version 2006

Motivation
Quite often some authors say they compare their PSO versions
to the "standard one" ... which is never the same!
So the idea is to define a real standard at least for one year, validated by some
researchers of the field, in particular James Kennedy and Maurice Clerc.
This PSO version does not intend to be the best one on the market (in particular there is
no adaptation of the swarm size nor of the coefficients) but simply very near of the
original version (1995) with just a few improvements based on some recent works.

So referring to "standard PSO 2006" would mean exactly this version with the default values
detailed below as,for example, referring to "standard PSO 2006 (w=0.8)" would mean almost
this version but with a non standard first cognitive/confidence coefficient.

Parameters
S := swarm size
K := maximum number of particles _informed_ by a given one
T := topology of the information links
w := first cognitive/confidence coefficient
c := second cognitive/confidence coefficient
R := random distribution of c
B := rule "to keep the particle in the box"

Equations
For each particle and each dimension
v(t+1) = w*v(t) + R(c)*(p(t)-x(t)) + R(c)*(g(t)-x(t))
x(t+1) = x(t) + v(t+1)
where
v(t) := velocity at time t
x(t) := position at time t
p(t) := best previous position of the particle
g(t) := best previous position of the informants of the particle

Default values
S = 10+2*sqrt(D) where D is the dimension of the search space
K = 3
T := randomly modified after each step if there has been no improvement
w = 1/(2*ln(2))
c = 0.5 + ln(2)
R = U(0..c), i.e. uniform distribution on [0, c]
B := set the position to the min. (max.) value and the velocity to zero

About information links topology
A lot of works have been done about this topic. The main result is there is no
"best" topology. Hence the random approach used here. Note that is does not mean that
each particle is informed by K ones: the number of particles that informs a given one
may be any value between 1 (for each particle informs itself) and S.

About initialisation
Initial positions are chosen at random inside the search space (which is
supposed to be a hyperparallelepid, and even often a hypercube), according to an uniform
distribution. This is not the best way, but the one of the original PSO.

Each initial velocity is simply defined as the difference of two random
positions. It is simple, and needs no additional parameter.
However, again, it is not the best approach. The resulting distribution is not even
uniform, as for any method that uses an uniform distribution independently for each
component. The mathematically correct approach needs to use an uniform
distribution inside a hypersphere. It is not that difficult, and indeed used in some PSO
versions, but quite different from the original one.

*/
