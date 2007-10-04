/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../SwarmFitterInterface.h"

// Standard PSO version 2006, for algorithm see end of file

FitterResults SwarmFitterInterface::runFitter(ModelTuningParameters * startPoint) {

	int numberOfFlies = (int)(10.0+2.0*sqrt((double)toInt(fixedParams["Dimensions"])));

	showMessage("Running Swarm Optimization with " + str(numberOfFlies) + " flies\n",3,fixedParams); 

	vector< ModelTuningParameters > results; 

	int maxInformed = 3;
	double w = 1.0/(2.0*log(2.0));
	double c = 0.5 + log(2.0);
	
	double tempBestValue = 0;
	///True if tempBestValue is initialized
	bool initBest = false; 

	int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);
	if (numberOfRuns < 0) crash("SwarmFitterInterface","Negative number of runs !!");

	MTRand randGen( toInt(fixedParams["Seed"]) );

	vector< SwarmFly > swarm(numberOfFlies, SwarmFly(w, c, &randGen, FixedParameters(fixedParams.getGlobals(),true)));

	vector< ModelTuningParameters > startPoints(numberOfFlies,ModelTuningParameters(*startPoint));

	ModelTuningParameters startX;
	ModelTuningParameters startY;

	//////////////////////////////////
	/// Initialize the swarm flies ///
	//////////////////////////////////
	for (int i = 0; i < numberOfFlies; i++) {
		startPoints[i] = swarm[i].calculateRandomPosition();				
	}
	
	errorValue->calculateParallelErrorValue(startPoints);
	results.insert(results.end(),startPoints.begin(),startPoints.end()); 


	for (int i = 0; i < numberOfFlies; i++) {
		swarm[i].setNewPositionErrorValue(startPoints[i]);
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

		errorValue->calculateParallelErrorValue(flyPositions);
		results.insert(results.end(),startPoints.begin(),startPoints.end()); 

		for (int j = 0; j < numberOfFlies; j++) {
			swarm[j].setNewPositionErrorValue(flyPositions[j]);
		}
		showMessage( "Best solution after run " + str(i) + " : " + SwarmFly::bestGlobalSolution.toString() + " : " + str(SwarmFly::bestGlobalSolution.getErrorValue()) + "\n",2,fixedParams);
		if (SwarmFly::bestGlobalSolution.getErrorValue() < tempBestValue || !initBest) {
			tempBestValue = SwarmFly::bestGlobalSolution.getErrorValue();
			initBest = true;
		}
		else {
			showMessage("No better solution found in the last run: Randomizing swarm topology\n",3,fixedParams); 
			//randomizeWorst(swarm);
			randomizeTopology(swarm, maxInformed, randGen);
		}

	}

	return FitterResults(results);
}

void SwarmFitterInterface::randomizeWorst(vector< SwarmFly > & swarm) {
	double worstValue = swarm[0].getErrorValue();
	int worstFly = 0;
	for (int i = 0; i < (int)swarm.size(); i++) {
		if (swarm[i].getErrorValue() > worstValue) {
			worstValue = swarm[i].getErrorValue();
			worstFly = i;
		};
	}

	showMessage("Randomizing worst fly in the swarm: "+swarm[worstFly].getParameters().toString()+"\n",3,fixedParams);
	
	swarm[worstFly].makeNextPositionRandom();

}

void SwarmFitterInterface::randomizeTopology(vector< SwarmFly > & swarm, int maxInformed, MTRand & randGen) {

	for (int i = 0; i < (int)swarm.size(); i++) {
		swarm[i].resetInformants();
		showMessage(str(i) + " informs: { ",4,fixedParams); 
		for (int j = 0; j < maxInformed; j++) {
			int randI = randGen.randInt(swarm.size()-1);
			swarm[randI].addInformant(&(swarm[j]));
			showMessage(str(randI) + " ",4,fixedParams); 
		}
		showMessage("}\n",4,fixedParams); 
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
