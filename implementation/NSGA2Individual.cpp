/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2Individual.h"

NSGA2Individual::NSGA2Individual(): rankSet(false), crowdDistanceSet(false) {}

NSGA2Individual::NSGA2Individual(ModelTuningParameters params): rankSet(false), crowdDistanceSet(false), tuningParameters(params) {}


bool NSGA2Individual::dominates(NSGA2Individual other) const {

	for (unsigned i = 0; i < getNumberOfObjectives(); i++) {
		if (other.getObjective(i) <= getObjective(i)) return false; 
	}	
	
	return true;

}

bool NSGA2Individual::crowdCompIsSmaller(NSGA2Individual other) const {

	if (!rankSet || !other.rankSet) crash("NSGA2Individual","Rank not set while evaluating crowded-comparator");
	if (rank < other.rank) {
		return true;
	}
	else if (rank == other.rank) {
		if (!crowdDistanceSet || !other.crowdDistanceSet) crash("NSGA2Individual","Crowding distance not set while evaluating domination of individuals with equal rank");
		return (crowdDistance > other.crowdDistance);
	}
	else {
		return false;
	}

}
    
void NSGA2Individual::setRank(unsigned newRank) {

	rank = newRank;
	rankSet = true;

}
    
unsigned NSGA2Individual::getRank() const {

	if (!rankSet) crash("NSGA2Individual","Rank not set in getRank()");
	return rank;

}

unsigned NSGA2Individual::getNumberOfObjectives() const {

	return tuningParameters.getNumberOfMOErrorValues();

}

double NSGA2Individual::getObjective(unsigned index) const {

	if (!errorValuesCalculated()) crash("NSGA2Individual","Trying to get uninitialized objective value, index: "+str(index));
	return tuningParameters.getMOErrorValue(index);

}

void NSGA2Individual::resetRank() {

	rankSet = false;
	crowdDistanceSet = false;	

}

bool NSGA2Individual::errorValuesCalculated() const {

	return tuningParameters.validErrorValue();	

}

ModelTuningParameters NSGA2Individual::getModelTuningParameters() {

	return tuningParameters;

}
    
void NSGA2Individual::setModelTuningParameters(ModelTuningParameters newParams) {

	tuningParameters = newParams;

}

void NSGA2Individual::setCrowdingDistance(double dist) {

	crowdDistance = dist;
	crowdDistanceSet = true;

}
    

double NSGA2Individual::getCrowdingDistance() const {

	if (!crowdDistanceSet) crash("NSGA2Individual","Crowding distance not set in getCrowdingDistance()");
	return crowdDistance;

}
    

bool NSGA2Individual::crowdingDistanceSet() const {

	return crowdDistanceSet;

}

string NSGA2Individual::toString() const {

	return tuningParameters.toString();

}
