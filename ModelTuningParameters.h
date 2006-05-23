#ifndef MODELTUNINGPARAMETERS_H
#define MODELTUNINGPARAMETERS_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

///todo remove this
#include "InputChannel.h"
#include "OutputChannel.h"

#include "Tools.h"

using namespace std;

class ModelTuningParameters {

public:
	ModelTuningParameters();
	ModelTuningParameters(const int);
	ModelTuningParameters(const vector< double > params, const int length, const vector< double > bounds);
	ModelTuningParameters(const string, const int length, const string bounds); //Read parameters from string like "5.0 14.0 1.0 8.0"

	int getLength() const;
	void setTuningParameters(const vector< double >, const int);
	void setTuningParameters(const string, const int);

	void setBounds(const vector< double >, const int boundsLength);
	void setBounds(const string, const int boundsLength);

	double getLowerBound(const int) const;
	double getUpperBound(const int) const;

	void setLowerBound(const int, const double);
	void setUpperBound(const int, const double);

	double &operator[](const int);
	const double &operator[](const int) const;

	void setFitnessValue(const double newValue);
	double getFitnessValue() const;

	string toString() const;

	void printOn(OutputChannel &) const;
	void readFrom(InputChannel &);


private:
	vector < double > tuningParameters;
	vector < double > bounds;
	
	//int tuningParametersLength;

	double fitnessValue;
};


#endif
