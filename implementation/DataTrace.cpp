#include "../DataTrace.h"

DataTrace::DataTrace() 
	:  points(vector< double >(0)), weight(-1), startTime(-1), stopTime(-1) {}

DataTrace::DataTrace(int size)
	: points(vector< double >(size)), weight(-1), startTime(-1), stopTime(-1) {}

int DataTrace::getLength() const {
	return points.size();
}

///
/// This will remove all existing data !!!
///
void DataTrace::resetAndSetLength(const int size) {
	points.clear();
	points.resize(size,0);
}

double DataTrace::getWeight() const {	
	if (weight == -1) crash("DataTrace","Weight not set");
	return weight;
}


double DataTrace::getStartTime() const {
	if (startTime == -1) crash("DataTrace","StartTime not set");
	return startTime;
}

double DataTrace::getStopTime() const {
	if (stopTime == -1) crash("DataTrace","StopTime not set");
	return stopTime;
}

double DataTrace::getSamplingFrequency() const{
	if (samplingFrequency == -1) crash("DataTrace","SamplingFrequency not set");
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

void DataTrace::printOn(ostream & output) const {
	output << points.size() << " ";
	for (unsigned i = 0; i < points.size(); i++) {
		output << points[i] << " ";
	}
	output << weight << " ";
	output << startTime << " ";
	output << stopTime << " ";
	output << samplingFrequency << " ";
	output << name.length() << " ";
	for (unsigned i = 0; i < name.length(); i++) {
		output << name[i] << " ";
	}

}

void DataTrace::readFrom(istream & input) {
	int length;
	input >> length;
	points = vector< double >(length);
	for (unsigned i = 0; i < points.size(); i++) {
		input >> points[i];
	}
	input >> weight;
	input >> startTime;
	input >> stopTime;
	input >> samplingFrequency;
	input >> length;
	name = string(length,' ');
	for (unsigned i = 0; i < name.length(); i++) {
		input >> name[i];
	}
}
    

double &DataTrace::operator[] (const int subscript) {
    if (subscript < 0 || subscript >= (int)points.size()) {
        crash("DataTrace","Invalid subscript");
    }
	return points[subscript];
}

const double &DataTrace::operator[] (const int subscript) const {
    if (subscript < 0 || (int)points.size()) {
        crash("DataTrace","Invalid subscript");
    }
	return points[subscript];
}

