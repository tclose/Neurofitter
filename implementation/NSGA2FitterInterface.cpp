/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2FitterInterface.h"

// Standard PSO version 2006, for algorithm see end of file

NSGA2FitterInterface::NSGA2FitterInterface(ErrorValueCalculator * fit, FixedParameters params) : FitterInterface(fit), FixedParamObject(params), parentPopulationSize(toUnsigned(fixedParams["ParentPopulationSize"]))  {}


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
		while (parents.getSize() + (population.getFront(rank)).size() <= parentPopulationSize) {		
			//calculateCrowdingDistance(population.getFront(rank))
			parents = parents.makeUnion(population.getFront(rank));
			rank++;
		}
		
		vector< NSGA2Individual > sortedFront = population.getSortedFront(rank);
		sortedFront.erase(sortedFront.begin()+(parentPopulationSize-parents.getSize()),sortedFront.end());
		parents = parents.makeUnion(sortedFront);
		children = parents.createChildren();
	}

	return FitterResults();
 
}
