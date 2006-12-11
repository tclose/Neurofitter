/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_DATATRACE_H
#define NEUROFITTER_DATATRACE_H

#include <string>
#include <vector>
#include <iostream>

#include "Tools.h"

#include "InputChannel.h"
#include "OutputChannel.h"

using namespace std;

class DataTrace {

public:
	DataTrace();
	DataTrace(const int size); 

	int getLength() const;

	double getWeight() const;
	double getStartTime() const;
	double getStopTime() const;
	double getSamplingFrequency() const;

	string getName() const;

	void setWeight(const double);
	void setStartTime(const double);
	void setStopTime(const double);
	void setSamplingFrequency(const double);
	void setName(const string);

	void resetAndSetLength(const int);

	void printOn(OutputChannel &) const;
	void readFrom(InputChannel &);

	double &operator[](const int);
	const double &operator[](const int) const;

protected:
	vector< double > points;
	
	double weight;
	double startTime;
	double stopTime;
	double samplingFrequency;

	string name;
};

#endif
