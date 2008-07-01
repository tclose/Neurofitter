/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2Population.h"

NSGA2Population::NSGA2Population(MTRand * rand): random(rand) {}

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

vector< NSGA2Individual > NSGA2Population::getIndividuals() const {

	return population;

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

NSGA2Population NSGA2Population::makeUnion(NSGA2Population other) const {

	return this->makeUnion(other.getIndividuals());

}

NSGA2Population NSGA2Population::makeUnion(vector< NSGA2Individual > other) const {

	NSGA2Population returnPopulation(*this);

	for (unsigned i = 0; i < other.size(); i++) {
		returnPopulation.addIndividual(other[i]);
	}

	return returnPopulation;

}

void NSGA2Population::addIndividual(NSGA2Individual ind) {

	classified = false;
	population.push_back(ind);

}

void NSGA2Population::addIndividuals(vector< NSGA2Individual > inds) {

	classified = false;
	for (unsigned i = 0; i < inds.size(); i++) {
		population.push_back(inds[i]);
	}

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

vector< NSGA2Individual > NSGA2Population::getFront(unsigned rankNumber) const {

	if (!classified) crash("NSGA2Population","Getting front size, but population is not classified");
	if (rankNumber > getMaxRank())  crash("NSGA2Population","Getting front size, but rank is out of range: "+str(rankNumber));
	vector< NSGA2Individual > front(fronts[rankNumber].size());
	for (unsigned i = 0; i < fronts[rankNumber].size(); i++) {
		front[i] = population[fronts[rankNumber][i]];
	}
	return front;
}

vector< NSGA2Individual > NSGA2Population::getSortedFront(unsigned rankNumber) const {

	vector< NSGA2Individual > front = getFront(rankNumber);
	NSGA2Individual tmp;
	for (unsigned i = 0; i < front.size(); i++) {
		for (unsigned j = i; j < front.size(); j++) {
			if (front[j].dominates(front[i])) {
				tmp = front[i];
				front[i] = front[j];
				front[j] = tmp;
			}
		}
	}

	return front;

}

unsigned NSGA2Population::getMaxRank() const {

	if (!classified) crash("NSGA2Population","Getting maximum rank, but population is not classified");
	return fronts.size();
}

NSGA2Population NSGA2Population::createChildren() const {

	NSGA2Population children;
	
	vector< pair< NSGA2Individual, NSGA2Individual > > selectedParents = this->selectParents();

	for (unsigned i = 0; i < selectedParents.size(); i++) {
		children.addIndividuals(mutate(mate(selectedParents[i])));
	}
	
	return children;

}

vector< pair< NSGA2Individual, NSGA2Individual > > NSGA2Population::selectParents() const {

	vector< pair< NSGA2Individual, NSGA2Individual > > parents;
	
	for (unsigned i = 0; i < population.size(); i++) {
		for (unsigned j = 0; j < 2; j++) {
			unsigned candidate1 = random->randInt(population.size());
			unsigned candidate2 = random->randInt(population.size());

			if (population.size() > 1) {
				while (candidate1 == candidate2) {
					candidate2 = random->randInt(population.size());
				}
			}
			
			NSGA2Individual winner;

			if (population[candidate1].dominates(population[candidate2])) {
				winner = population[candidate1];
			}	
			else {
				winner = population[candidate2];
			}

			if (j == 0) parents[i].first = winner; 
			else parents[i].second = winner;
		}
	}
	return parents;
}

vector< NSGA2Individual > NSGA2Population::mate(pair< NSGA2Individual, NSGA2Individual > parents) const {

	ModelTuningParameters parent1 = parents.first.getModelTuningParameters();
	ModelTuningParameters parent2 = parents.second.getModelTuningParameters();

	ModelTuningParameters child1(parent1);
	child1.resetErrorValue();
	ModelTuningParameters child2(parent2);
	child2.resetErrorValue();

	for (unsigned i = 0; i < parent1.getLength(); i++) {
		double u = random->rand(); //Random [0,1]

		double eta = 0.5;

		double beta;

		if (u <= 0.5) {
			beta = pow(2*u,1/(eta+1));
		}
		else {
			beta = pow(1/(2*(1-u)),1/(eta+1));
		}

		child1[i] = 0.5*((1+beta)*parent1[i]+(1-beta)*parent2[i]); 
		child2[i] = 0.5*((1-beta)*parent1[i]+(1+beta)*parent2[i]);

	}
	
	vector< NSGA2Individual > children;
	children.push_back(NSGA2Individual(child1));
	children.push_back(NSGA2Individual(child2));

	return children;

}

vector< NSGA2Individual > NSGA2Population::mutate(vector< NSGA2Individual > inds) const {

	vector< NSGA2Individual > newInds;

	for (unsigned i = 0; i < inds.size(); i++) {
		ModelTuningParameters individual = inds[i].getModelTuningParameters();

		for (unsigned j = 0; j < individual.getLength(); j++) {

			double u = random->rand(); //Random [0,1]

			double eta = 0.5;

			double delta;
			if (u < 0.5) {
				delta = pow(2*u,1/(eta+1)) - 1;
			}
			else {
				delta = 1 - pow(2*(1-u),1/(eta+1));
			}

			individual[j] = individual[j] +(individual.getUpperBound(j)-individual.getLowerBound(j))*delta;
		}
		
		newInds.push_back(NSGA2Individual(individual));
	}

	return newInds;	

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
