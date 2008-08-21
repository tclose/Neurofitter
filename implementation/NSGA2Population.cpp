/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2Population.h"

NSGA2Population::NSGA2Population(FixedParameters params): FixedParamObject(params) {}

NSGA2Population::NSGA2Population(MTRand * rand, FixedParameters params): FixedParamObject(params), random(rand) {}

void NSGA2Population::classify() {

	showMessage("Classifying the population in fronts\n",10,fixedParams);

	fronts.clear();
	dominationList = vector< vector< unsigned > >(population.size());
	dominationNumber = vector< unsigned >(population.size());

	for (unsigned i = 0; i < population.size(); i++) {
		NSGA2Individual & ind1 = population[i];

		dominationNumber[i] = 0; //number of individuals that dominate ind1
	
		for (unsigned j = 0; j < population.size(); j++) {
			if (i != j) {
				NSGA2Individual & ind2 = population[j];
				if (ind1.dominates(ind2)) {
					showMessage("Individual "+ind1.toString()+" dominates "+ind2.toString()+"\n",24,fixedParams);
					dominationList[i].push_back(j);
				}
				else if (ind2.dominates(ind1)) {
					dominationNumber[i]++;
				}
			}
		}	

		if (dominationNumber[i]==0) {
			ind1.setRank(0);
			showMessage("Assigning rank 0 to : "+ind1.toString()+"\n",24,fixedParams);
			if (fronts.size() == 0) {
				fronts.push_back(vector< unsigned >(0));
			}
			fronts[0].push_back(i);
		}
	}

	int rank = 0;
	while (fronts.size() != 0 && fronts[rank].size() != 0) {
		showMessage("Starting new front with rank: "+str(rank+1)+"\n",14,fixedParams);
		fronts.push_back(vector< unsigned >(0));

		for (unsigned i = 0; i < fronts[rank].size(); i++) {
			unsigned ind1Number = fronts[rank][i];
			for (unsigned j = 0; j < dominationList[ind1Number].size(); j++) {
				unsigned ind2Number = dominationList[ind1Number][j];
				dominationNumber[ ind2Number ] --;
				if (dominationNumber[ ind2Number ] == 0) {
					population[ind2Number].setRank(rank+1);
					fronts[rank+1].push_back(ind2Number);
					showMessage("Assigning rank "+str(rank+1)+" to : "+population[ind2Number].toString()+"\n",24,fixedParams);
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
		population[i].resetRank();
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

void NSGA2Population::declassify() {

	for (unsigned i = 0; i < population.size(); i++) {
		population[i].resetRank();
	}

	classified = false;

}

NSGA2Population NSGA2Population::makeUnion(vector< NSGA2Individual > other) const {

	NSGA2Population returnPopulation(*this);

	for (unsigned i = 0; i < other.size(); i++) {
		//other[i].resetRank();
		returnPopulation.addIndividual(other[i]);
	}

	returnPopulation.classified = false;

	return returnPopulation;

}

void NSGA2Population::addIndividual(NSGA2Individual ind) {

	classified = false;
	//ind.resetRank();
	population.push_back(ind);

}

void NSGA2Population::addIndividuals(vector< NSGA2Individual > inds) {

	classified = false;
	for (unsigned i = 0; i < inds.size(); i++) {
		//inds[i].resetRank();
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
	if ((int)rankNumber > getMaxRank())  return 0;
	return fronts[rankNumber].size();
	
}

vector< NSGA2Individual > NSGA2Population::getFront(unsigned rankNumber) const {

	if (!classified) crash("NSGA2Population","Getting front, but population is not classified");
	if ((int)rankNumber > getMaxRank()) return vector< NSGA2Individual >(0);
	vector< NSGA2Individual > front(fronts[rankNumber].size());
	for (unsigned i = 0; i < fronts[rankNumber].size(); i++) {
		cout.flush();	
		front[i] = population[fronts[rankNumber][i]];
	}
	return front;
}

vector< NSGA2Individual > NSGA2Population::getSortedFront(unsigned rankNumber) const {

	vector< NSGA2Individual > front = getFront(rankNumber);
	NSGA2Individual tmp;
	for (unsigned i = 0; i < front.size(); i++) {
		for (unsigned j = i; j < front.size(); j++) {
			if (front[j].crowdCompIsSmaller(front[i])) {
				tmp = front[i];
				front[i] = front[j];
				front[j] = tmp;
			}
		}
	}

	return front;

}

int NSGA2Population::getMaxRank() const {

	if (!classified) crash("NSGA2Population","Getting maximum rank, but population is not classified");
	return fronts.size()-1;
}

NSGA2Population NSGA2Population::createChildren() const {

	NSGA2Population children;
	
	vector< pair< NSGA2Individual, NSGA2Individual > > selectedParents = this->selectParents();

	for (unsigned i = 0; i < selectedParents.size(); i++) {
		children.addIndividuals(mutate(mate(selectedParents[i])));
	}

	for (unsigned i = 0; i < children.population.size(); i++) {
		children.population[i].resetRank();
	}
	
	return children;

}

vector< pair< NSGA2Individual, NSGA2Individual > > NSGA2Population::selectParents() const {

	showMessage("Selecting parents\n",14,fixedParams);

	vector< pair< NSGA2Individual, NSGA2Individual > > parents(population.size());
	
	for (unsigned i = 0; i < parents.size(); i++) {
		vector< NSGA2Individual > winner(2);

		for (unsigned j = 0; j < 2; j++) {
			unsigned candidate1 = random->randInt(population.size()-1);
			unsigned candidate2 = random->randInt(population.size()-1);

			if (population.size() > 1) {
				while (candidate1 == candidate2) {
					candidate2 = random->randInt(population.size()-1);
				}
			}
			

			if (population[candidate1].crowdCompIsSmaller(population[candidate2])) {
				winner[j] = population[candidate1];
			}	
			else {
				winner[j] = population[candidate2];
			}
		}
		parents[i]=pair< NSGA2Individual, NSGA2Individual >(winner[0], winner[1]);
	}
	return parents;
}

vector< NSGA2Individual > NSGA2Population::mate(pair< NSGA2Individual, NSGA2Individual > parents) const {


	showMessage("Mating 2 parents "+parents.first.toString()+", "+parents.second.toString(),14,fixedParams);

	ModelTuningParameters parent1 = parents.first.getModelTuningParameters();
	ModelTuningParameters parent2 = parents.second.getModelTuningParameters();

	ModelTuningParameters child1(parent1);
	child1.resetErrorValue();
	//ModelTuningParameters child2(parent2);
	//child2.resetErrorValue();

	for (unsigned i = 0; i < parent1.getLength(); i++) {
		double u = random->rand(); //Random [0,1]

		double eta = toDouble(fixedParams["EtaCrossover"]);;

		double beta;

		if (u <= 0.5) {
			beta = pow(2*u,1/(eta+1));
		}
		else {
			beta = pow(1/(2*(1-u)),1/(eta+1));
		}

		child1[i] = 0.5*((1+beta)*parent1[i]+(1-beta)*parent2[i]); 
		//child2[i] = 0.5*((1-beta)*parent1[i]+(1+beta)*parent2[i]);

		if (child1[i] > child1.getUpperBound(i)) child1[i] = child1.getUpperBound(i);
		//if (child2[i] > child2.getUpperBound(i)) child2[i] = child2.getUpperBound(i);
		if (child1[i] < child1.getLowerBound(i)) child1[i] = child1.getLowerBound(i);
		//if (child2[i] < child2.getLowerBound(i)) child2[i] = child2.getLowerBound(i);

	}
	
	vector< NSGA2Individual > children;
	children.push_back(NSGA2Individual(child1));
	//children.push_back(NSGA2Individual(child2));

	//showMessage(" to create 2 children: "+child1.toString()+", "+child2.toString()+"\n",14,fixedParams);
	showMessage(" to create a child: "+child1.toString()+"\n",14,fixedParams);

	return children;

}

vector< NSGA2Individual > NSGA2Population::mutate(vector< NSGA2Individual > inds) const {

	showMessage("Mutating "+str((unsigned)inds.size())+" children",14,fixedParams);

	vector< NSGA2Individual > newInds;

	for (unsigned i = 0; i < inds.size(); i++) {
		ModelTuningParameters individual = inds[i].getModelTuningParameters();

		for (unsigned j = 0; j < individual.getLength(); j++) {

			double u1 = random->rand(); //Random [0,1]
			double u2 = random->rand(); //Random [0,1]

			double pMutation = toDouble(fixedParams["pMutation"]);;
			double eta = toDouble(fixedParams["EtaMutation"]);;

			double delta;
			
			
			if (u1 < pMutation) {
				if (u2 < 0.5) {
					delta = pow(2*u2,1/(eta+1)) - 1;
				}
				else {
					delta = 1 - pow(2*(1-u2),1/(eta+1));
				}
			}
			else {
				delta = 0;
			}

			individual[j] = individual[j] + (individual.getUpperBound(j)-individual.getLowerBound(j))*delta;
			if (individual[j] > individual.getUpperBound(j)) individual[j] = individual.getUpperBound(j);
			if (individual[j] < individual.getLowerBound(j)) individual[j] = individual.getLowerBound(j);

		}
		
		newInds.push_back(NSGA2Individual(individual));
	}

	string mutatedString;
 	for (unsigned i = 0; i < newInds.size(); i++) {
		mutatedString += newInds[i].toString() + ", ";
	}

	showMessage(" into : "+mutatedString+"\n",14,fixedParams);

	return newInds;	

}

void NSGA2Population::calculateErrorValues(ErrorValueCalculator * errorValueCalc) {

	vector< ModelTuningParameters > parameters;    
    
    for (unsigned i = 0; i < population.size(); i++) {
		if (!population[i].errorValuesCalculated()) {
			parameters.push_back(population[i].getModelTuningParameters()); 
		}
	}
 
	errorValueCalc->calculateParallelErrorValue(parameters);

    int nEval = 0;

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

void NSGA2Population::calculateFrontCrowdingDistance(unsigned rank) {

	showMessage("Calculating crowding distances\n",14,fixedParams);

	if (!classified) crash("NSGA2Population","Calculating crowding distance, but fronts not classified");	
	if ((int)rank > getMaxRank()) crash("NSGA2Population","Index of front too high: "+str(rank));	

	vector< NSGA2Individual * > front(fronts[rank].size());
	for (unsigned i = 0; i < front.size(); i++) {
		front[i] = &(population[fronts[rank][i]]);
		front[i]->setCrowdingDistance(0);
	}

	if (front.size() > 0) {

		for (unsigned i = 0; i < front[0]->getNumberOfObjectives(); i++) {
			NSGA2Individual * tmp;
			for (unsigned j = 0; j < front.size(); j++) {
				for (unsigned k = j; k < front.size(); k++) {
					if (front[k]->getObjective(i) < front[j]->getObjective(i)) {
						tmp = front[j];
						front[j] = front[k];
						front[k] = tmp;
					}
				}
			}

			vector< NSGA2Individual * >::iterator it;

			(*(front.begin()))->setCrowdingDistance(numeric_limits< double >::max());
			(*(front.rbegin()))->setCrowdingDistance(numeric_limits< double >::max());

			for (it = front.begin()+1; it < front.end()-1; it++) {
				if ((*it)->getCrowdingDistance() != numeric_limits< double >::max()) {
					(*it)->setCrowdingDistance((*it)->getCrowdingDistance() + ((*(it+1))->getObjective(i) - (*(it-1))->getObjective(i)));
					///todo HAVE TO FIND A BETTER SOLUTION /(templ.getUpperBound(i)-templ.getLowerBound(i)));
				}
			}
		}

	}	

}
