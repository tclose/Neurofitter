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
	ModelTuningParameters(const string, const int length, const string bounds); //Read parameters from string like "5.0 14.0 1.0 8.0"
	ModelTuningParameters(const ModelTuningParameters & t) {tuningParameters = NULL;bounds = NULL;*this=t;};
	
	~ModelTuningParameters();

	ModelTuningParameters& operator=(const ModelTuningParameters &);

	double * getTuningParameters() const;
	int getLength() const;
	void setTuningParameters(const double*, const int);
	void setTuningParameters(const string, const int);

	void setBounds(const double*, const int boundsLength);
	void setBounds(const string, const int boundsLength);

	double getLowerBound(const int) const;
	double getUpperBound(const int) const;

	void setLowerBound(const int, const double);
	void setUpperBound(const int, const double);

	double &operator[](const int);
	const double &operator[](const int) const;

	string toString() const;
	

private:
	double * tuningParameters;
	double * bounds;
	double * getBounds() const;
	int tuningParametersLength;
	

};

#endif
