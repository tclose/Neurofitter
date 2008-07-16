/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_NSGA2POPULATION_H
#define NEUROFITTER_NSGA2POPULATION_H


#include <vector>
#include <limits>

#include "FixedParamObject.h"
#include "NSGA2Individual.h"
#include "ErrorValueCalculator.h"
#include "MersenneTwister.h"

using namespace std;

class NSGA2Individual;

class NSGA2Population : public FixedParamObject {
	
public:

	NSGA2Population() {};
	NSGA2Population(FixedParameters);
	NSGA2Population(MTRand * rand, FixedParameters);

	void classify();
	bool isClassified() const;
	unsigned getSize() const;
	void initialize(ModelTuningParameters,unsigned);

	NSGA2Individual get(unsigned index) const;
	NSGA2Population makeUnion(NSGA2Population other) const;
	NSGA2Population makeUnion(vector< NSGA2Individual >) const;
	void addIndividual(NSGA2Individual);
	void addIndividuals(vector< NSGA2Individual >);
	unsigned getFrontSize(unsigned rank) const;
	vector< NSGA2Individual > getFront(unsigned rank) const;
	vector< NSGA2Individual > getSortedFront(unsigned rank) const;
	vector< NSGA2Individual > getIndividuals() const;
	int getMaxRank() const;
	void clear();
	NSGA2Population createChildren() const;
	void calculateErrorValues(ErrorValueCalculator * errorValueCalc);
	void calculateFrontCrowdingDistance(unsigned rank);

private:
	bool classified;
	unsigned offspringSize;
	MTRand * random;
	vector< NSGA2Individual > population;
	vector< vector< unsigned > > fronts;
	vector< vector< unsigned > > dominationList;
	vector< unsigned > dominationNumber;

	void initIndividual(NSGA2Individual &);
	vector< NSGA2Individual > mate(pair< NSGA2Individual, NSGA2Individual > parents) const;
	vector< pair< NSGA2Individual, NSGA2Individual > > selectParents() const;
	vector< NSGA2Individual > mutate(vector< NSGA2Individual >) const;
};

#endif
