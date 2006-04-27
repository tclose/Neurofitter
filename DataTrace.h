#ifndef DATATRACE_H
#define DATATRACE_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

///todo use vector to implement this
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


	double &operator[](const int);
	const double &operator[](const int) const;

private:
	vector< double > points;
	int length;
	double weight;
	double startTime;
	double stopTime;
	double samplingFrequency;


	string name;
};

#endif
