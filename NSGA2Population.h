/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_NSGA2POPULATION_H
#define NEUROFITTER_NSGA2POPULATION_H


#include <vector>

#include "FixedParamObject.h"
#include "NSGA2Individual.h"
#include "ErrorValueCalculator.h"
#include "MersenneTwister.h"

using namespace std;

class NSGA2Individual;

class NSGA2Population : public FixedParamObject {
	
public:

	NSGA2Population() {};
	NSGA2Population(MTRand * rand);
	NSGA2Population(vector< NSGA2Individual * >);

	void classify();
	bool isClassified() const;
	unsigned getSize() const;
	void initialize(ModelTuningParameters,unsigned);

	NSGA2Individual get(unsigned index) const;
	NSGA2Population makeUnion(NSGA2Population other);
	NSGA2Population makeUnion(vector< NSGA2Individual * >);
	unsigned getFrontSize(unsigned rank) const;
	vector< NSGA2Individual * > getFront(unsigned rank);
	vector< NSGA2Individual * > getIndividuals();
	unsigned getMaxRank() const;
	void clear();
	NSGA2Population createChildren() const;
	void calculateErrorValues(ErrorValueCalculator * errorValueCalc);

private:
	bool classified;
	MTRand * random;
	vector< NSGA2Individual > population;
	vector< vector< int > > fronts;
	vector< vector< int > > dominationList;
	vector< int > dominationNumber;

	void initIndividual(NSGA2Individual &);
};

#endif
