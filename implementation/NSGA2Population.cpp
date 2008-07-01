/*
Revision of last commit: $Rev: 227 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-18 13:29:41 +0900 (Thu, 18 Oct 2007) $
*/

#include "../NSGA2Population.h"

// Standard PSO version 2006, for algorithm see end of file


NSGA2Population::NSGA2Population(MTRand * rand): random(rand) {}

//NSGA2Population::NSGA2Population(vector< NSGA2Individual * > individuals): population(vector< NSGA2Individual >(individuals.size())) {}

void NSGA2Population::classify() {

	fronts.clear();
	dominationList = vector< vector< int > >(population.size());
	dominationNumber = vector< int >(population.size());

	for (unsigned i = 0; i < population.size(); i++) {
		NSGA2Individual & ind1 = population[i];

		dominationNumber[i] = 0; //number of individuals that dominate ind1
	
		for (unsigned j = 0; j < population.size(); j++) {
			NSGA2Individual & ind2 = population[j];
			if (ind1.dominates(ind2)) {
				dominationList[i].push_back(j);
			}
			else {
				dominationNumber[i]++;
			}
		}	

		if (dominationNumber[i]==0) {
			ind1.setRank(1);
		}
		else {
			if (fronts.size() == 0) {
				fronts.push_back(vector< int >(0));
			}
			fronts[0].push_back(i);
		}
	}

	int rank = 0;
	while (fronts.size() != 0 && fronts[rank].size() != 0) {
		fronts.push_back(vector< int >(0));

		for (unsigned i = 0; i < fronts[rank].size(); i++) {
			unsigned ind1Number = fronts[rank][i];
			for (unsigned j = 0; j < dominationList[ind1Number].size(); j++) {
				unsigned ind2Number = dominationList[ind1Number][j];
				dominationNumber[ ind2Number ] --;
				if (dominationNumber[ ind2Number ] == 0) {
					population[ind2Number].setRank(rank+1);
					fronts[rank+1].push_back(ind2Number);
				}
			}
		}		
		rank++;
	}

	classified = true;

}

bool NSGA2Population::isClassified() const {

	return classified;	

}

vector< NSGA2Individual * > NSGA2Population::getIndividuals() {

	vector< NSGA2Individual * > returnVector(population.size());
	for (unsigned i = 0; i < population.size(); i++) {
		returnVector[i] = &(population[i]);
	}

	return returnVector;

}

void NSGA2Population::initialize(ModelTuningParameters templ, unsigned size) {

	population = vector< NSGA2Individual >(size, templ);
	for (unsigned i = 0; i < size; i++) {
		initIndividual(population[i]);
	}

}


void NSGA2Population::clear() {

	classified = false;
	population.clear();
	fronts.clear();
	dominationList.clear();
	dominationNumber.clear();

}

NSGA2Population NSGA2Population::makeUnion(NSGA2Population other) {

	return this->makeUnion(other.getIndividuals());

}

NSGA2Population NSGA2Population::makeUnion(vector< NSGA2Individual * > other) {

	return NSGA2Population();

}

unsigned NSGA2Population::getSize() const {

	return population.size();		

}

NSGA2Individual NSGA2Population::get(unsigned int index) const {

	if (index >= population.size()) crash("NSGA2Population","Getting individual : index out of range: "+str(index));
    return population[index];

}

unsigned NSGA2Population::getFrontSize(unsigned rankNumber) const {

	if (!classified) crash("NSGA2Population","Getting front size, but population is not classified");
	if (rankNumber > getMaxRank())  crash("NSGA2Population","Getting front size, but rank is out of range: "+str(rankNumber));
	return fronts[rankNumber].size();
	
}

vector< NSGA2Individual * > NSGA2Population::getFront(unsigned rankNumber) {

	if (!classified) crash("NSGA2Population","Getting front size, but population is not classified");
	if (rankNumber > getMaxRank())  crash("NSGA2Population","Getting front size, but rank is out of range: "+str(rankNumber));
	vector< NSGA2Individual * > front(fronts[rankNumber].size());
	for (unsigned i = 0; i < fronts[rankNumber].size(); i++) {
		front[i] = &population[fronts[rankNumber][i]];
	}
	return front;
}

unsigned NSGA2Population::getMaxRank() const {

	if (!classified) crash("NSGA2Population","Getting maximum rank, but population is not classified");
	return fronts.size();
}

NSGA2Population NSGA2Population::createChildren() const {

	return NSGA2Population();

	//matingselection
	//mating
	//mutation

}

void NSGA2Population::calculateErrorValues(ErrorValueCalculator * errorValueCalc) {

    int nEval = 0;
	vector< ModelTuningParameters > parameters;    
    
    for (unsigned i = 0; i < population.size(); i++) {
		if (!population[i].errorValuesCalculated()) {
			parameters[nEval++] = population[i].getModelTuningParameters(); 
		}
	}
 
	errorValueCalc->calculateParallelErrorValue(parameters);

    for (unsigned i = 0; i < population.size(); i++ ) {
		if (!population[i].errorValuesCalculated()) {
			population[i].setModelTuningParameters(parameters[nEval++]);
		}		
	}


} 

void NSGA2Population::initIndividual(NSGA2Individual & ind) {

	ModelTuningParameters parameters = ind.getModelTuningParameters();

	parameters.resetErrorValue();

    for (unsigned i = 0; i < parameters.getLength(); i++) {
        parameters[i] = parameters.getLowerBound(i)+random->rand(parameters.getUpperBound(i)-parameters.getLowerBound(i));
    }

	ind.setModelTuningParameters(parameters);

}
