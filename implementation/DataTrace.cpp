#include "../DataTrace.h"

DataTrace::DataTrace() 
	:  points(NULL),length(0), weight(-1), startTime(-1), stopTime(-1) {}

DataTrace::DataTrace(int size)
	:length(size), weight(-1), startTime(-1), stopTime(-1) {
	length = size;
	points = new double[size];
	if (points == NULL) {cerr << "Error: Unable to allocate memory in DataTrace";exit(1);}
}

DataTrace & DataTrace::operator=(const DataTrace & d) {
	if (this != &d) {
		if (points != NULL) {delete [] points;}
		length = d.length;
		points = new double [length];
		for (int i=0; i < length; i++) {
        	(*this)[i] = d[i];
    	} 
		weight = d.weight;
		startTime = d.startTime;
		stopTime = d.stopTime;
		name = d.name;
		samplingFrequency = d.samplingFrequency;
	}
	return *this;
}

DataTrace::~DataTrace() {
	if (points != NULL) {delete [] points;}
}


int DataTrace::getLength() const {
	return length;
}

///
/// This will remove all existing data !!!
///
void DataTrace::resetAndSetLength(const int size) {
	if (points != NULL) {
		delete [] points;
	}
	points = new double[size];
	if (points == NULL) {cerr << "Error: Unable to allocate memory in DataTrace";exit(1);}
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

