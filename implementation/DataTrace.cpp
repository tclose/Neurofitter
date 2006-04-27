#include "../DataTrace.h"

DataTrace::DataTrace() 
	:  points(vector< double >(0)),length(0), weight(-1), startTime(-1), stopTime(-1) {}

DataTrace::DataTrace(int size)
	: points(vector< double >(size)), length(size), weight(-1), startTime(-1), stopTime(-1) {}

int DataTrace::getLength() const {
	return length;
}

///
/// This will remove all existing data !!!
///
void DataTrace::resetAndSetLength(const int size) {
	points.clear();
	points.resize(size,0);
	length = size;
}

double DataTrace::getWeight() const {	
	if (weight == -1) {cerr << "Error: Weight of DataTrace not set";exit(1);}
	return weight;
}


double DataTrace::getStartTime() const {
	if (startTime == -1) {cerr << "Error: StartTime of DataTrace not set";exit(1);}
	return startTime;
}

double DataTrace::getStopTime() const {
	if (stopTime == -1) {cerr << "Error: StopTime of DataTrace not set";exit(1);}
	return stopTime;
}

double DataTrace::getSamplingFrequency() const{
	if (samplingFrequency == -1) {cerr << "Error: SamplingFrequency of DataTrace not set";exit(1);}
	return samplingFrequency;
}

string DataTrace::getName() const {
	return name;
}

void DataTrace::setStartTime(const double newStartTime) {
	startTime = newStartTime;
}
    
void DataTrace::setStopTime(const double newStopTime) {
	stopTime = newStopTime;	
}
    
void DataTrace::setSamplingFrequency(const double newFrequency) {
	samplingFrequency = newFrequency;
}


void DataTrace::setWeight(const double newWeight) {
	weight = newWeight;
}

void DataTrace::setName(const string newName) {
	name = newName;
}

double &DataTrace::operator[] (const int subscript) {
    if (0 <= subscript && subscript < length)
        {return points[subscript];}
    else
        {cerr << "Invalid subscript in DataTrace";exit(1);}
}

const double &DataTrace::operator[] (const int subscript) const {
    if (0 <= subscript && subscript < length)
        {return points[subscript];}
    else
        {cerr << "Invalid subscript in DataTrace";exit(1);}
}

