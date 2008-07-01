/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_NSGA2INDIVIDUAL_H
#define NEUROFITTER_NSGA2INDIVIDUAL_H

#include "FixedParamObject.h"
#include "ModelTuningParameters.h"

using namespace std;

class NSGA2Population;

class NSGA2Individual : public FixedParamObject {

public:
	NSGA2Individual();
	NSGA2Individual(ModelTuningParameters);
	bool dominates(NSGA2Individual other) const;
	void setRank(unsigned rank);
	unsigned getRank() const;
	double getObjective(unsigned index);
	bool errorValuesCalculated();
	ModelTuningParameters getModelTuningParameters();
	void setModelTuningParameters(ModelTuningParameters);

private:
	int rank;
	double crowdDistance;
	bool rankSet;
	bool crowdDistanceSet;
	ModelTuningParameters tuningParameters;
};

#endif
