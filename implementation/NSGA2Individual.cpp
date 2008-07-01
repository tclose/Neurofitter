/*
Revision of last commit: $Rev: 227 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-18 13:29:41 +0900 (Thu, 18 Oct 2007) $
*/

#include "../NSGA2Individual.h"

// Standard PSO version 2006, for algorithm see end of file

NSGA2Individual::NSGA2Individual(): rankSet(false), crowdDistanceSet(false) {}

NSGA2Individual::NSGA2Individual(ModelTuningParameters params): rankSet(false), crowdDistanceSet(false), tuningParameters(params) {}

bool NSGA2Individual::dominates(NSGA2Individual other) {

	if (!rankSet || !other.rankSet) crash("NSGA2Individual","Rank not set while evaluating domination");
	if (rank < other.rank) {
		return true;
	}
	else if  (rank == other.rank) {
		if (!crowdDistanceSet || !other.crowdDistanceSet) crash("NSGA2Individual","Crowding distnace not set while evaluating domination of individuals with equal rank");
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

double NSGA2Individual::getObjective(unsigned index) {

	if (!errorValuesCalculated()) crash("NSGA2Individual","Trying to get uninitialized objective value, index: "+str(index));
	return tuningParameters.getMOErrorValue(index);

}


bool NSGA2Individual::errorValuesCalculated() {

	return tuningParameters.validErrorValue();	

}

ModelTuningParameters NSGA2Individual::getModelTuningParameters() {

	return tuningParameters;

}
    
void NSGA2Individual::setModelTuningParameters(ModelTuningParameters newParams) {

	tuningParameters = newParams;

}

