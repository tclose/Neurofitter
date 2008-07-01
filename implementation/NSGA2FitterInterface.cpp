/*
Revision of last commit: $Rev: 227 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-18 13:29:41 +0900 (Thu, 18 Oct 2007) $
*/

#include "../NSGA2FitterInterface.h"

// Standard PSO version 2006, for algorithm see end of file

NSGA2FitterInterface::NSGA2FitterInterface(ErrorValueCalculator * fit, FixedParameters params) : FitterInterface(fit), FixedParamObject(params) {}


FitterResults NSGA2FitterInterface::runFitter(ModelTuningParameters * startPoint) {

	MTRand randGen( toInt(fixedParams["Seed"]) );

	population=NSGA2Population(&randGen);
	parents=NSGA2Population(&randGen);
	children=NSGA2Population(&randGen);

	parents.initialize(*startPoint, 50);
	population.initialize(*startPoint, 0);
	children.initialize(*startPoint, 0);

	while(true) {
		population = parents.makeUnion(children);
		population.calculateErrorValues(errorValue);
		population.classify();
		parents.clear();
		
		int rank = 0;
		while (parents.getSize() + (population.getFront(rank)).size() <= population.getSize()) {		
			//calculateCrowdingDistance(population.getFront(rank))
			parents = parents.makeUnion(population.getFront(rank));
			rank++;
		}
		
		vector< NSGA2Individual * > sortedFront = sortFront(population.getFront(rank));
		parents = parents.makeUnion(sortedFront);
		children = parents.createChildren();

	}

	return FitterResults();
 
}

vector< NSGA2Individual * > NSGA2FitterInterface::sortFront(vector< NSGA2Individual * > front) const {

	return vector< NSGA2Individual * >(0);

}
