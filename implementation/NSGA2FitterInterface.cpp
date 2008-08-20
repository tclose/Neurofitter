/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2FitterInterface.h"

NSGA2FitterInterface::NSGA2FitterInterface(ErrorValueCalculator * fit, FixedParameters params) : FitterInterface(fit), FixedParamObject(params), parentPopulationSize(toUnsigned(fixedParams["ParentPopulationSize"]))  {}


FitterResults NSGA2FitterInterface::runFitter(ModelTuningParameters * startPoint) {

	MTRand randGen( toInt(fixedParams["Seed"]) );

	population=NSGA2Population(&randGen,fixedParams);
	parents=NSGA2Population(&randGen, fixedParams);
	children=NSGA2Population(&randGen, fixedParams);

	parents.initialize(*startPoint, parentPopulationSize);
	population.initialize(*startPoint, 0);
	children.initialize(*startPoint, parentPopulationSize);

	while(true) {
		population = parents.makeUnion(children);
		population.declassify();

		population.calculateErrorValues(errorValue);
		population.classify();
		parents.clear();
		
		int rank = 0;
		while ((population.getFront(rank)).size() != 0 && parents.getSize() + (population.getFront(rank)).size() <= parentPopulationSize) {
			population.calculateFrontCrowdingDistance(rank);
			parents = parents.makeUnion(population.getFront(rank));
			rank++;
		}

		population.calculateFrontCrowdingDistance(rank);		
		vector< NSGA2Individual > sortedFront = population.getSortedFront(rank);
		sortedFront.erase(sortedFront.begin()+(parentPopulationSize-parents.getSize()),sortedFront.end());
		parents = parents.makeUnion(sortedFront);
		children = parents.createChildren();
	}

	return FitterResults();
 
}
