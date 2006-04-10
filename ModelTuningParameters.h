#ifndef MODELTUNINGPARAMETERS_H
#define MODELTUNINGPARAMETERS_H

#include <string>
#include <iostream>
#include <sstream>

#include "Tools.h"

using namespace std;


class ModelTuningParameters {

public:
	ModelTuningParameters();
	ModelTuningParameters(const int);
	ModelTuningParameters(const double* params, const int length, const double* bounds);
	ModelTuningParameters(const ModelTuningParameters & t) {tuningParameters = NULL;*this=t;};
	
	~ModelTuningParameters();

	ModelTuningParameters& operator=(const ModelTuningParameters &);

	double * getTuningParameters() const;
	int getLength() const;
	void setTuningParameters(const double*, const int);
	
	void setBounds(const double*, int boundsLength);
	double getLowerBound(const int) const;
	double getUpperBound(const int) const;

	void setLowerBound(const int, const double);
	void setUpperBound(const int, const double);

	double * getBounds() const;

	double &operator[](const int);
	const double &operator[](const int) const;

	string toString() const;
	

private:
	double * tuningParameters;
	double * bounds;
	int tuningParametersLength;
	

};

#endif
