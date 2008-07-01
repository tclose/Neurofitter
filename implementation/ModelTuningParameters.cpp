/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../ModelTuningParameters.h"

ModelTuningParameters::ModelTuningParameters() : 
	tuningParameters(), bounds(), errorValueIsValid(false) {}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) 
	: tuningParameters(newTParamsLength), bounds(), errorValueIsValid(false) {
}

ModelTuningParameters::ModelTuningParameters(const vector< double > newTParams, const int newTParamsLength, const vector < double > newBounds) : 
	tuningParameters(newTParamsLength), bounds(), errorValueIsValid(false) {

	ModelTuningParameters::setTuningParameters(newTParams);
	ModelTuningParameters::setBounds(newBounds);
}

ModelTuningParameters::ModelTuningParameters(const string paramString, const int newTParamsLength, const string newBounds) : 
	tuningParameters(newTParamsLength), bounds(), errorValueIsValid(false) {

	ModelTuningParameters::setTuningParameters(paramString, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength, const string newBounds) : 
	tuningParameters(newTParamsLength), bounds(), errorValueIsValid(false) {

	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
}

double ModelTuningParameters::getLowerBound(const int subscript) const {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {
		crash("ModelTuningParameters","Lower bound subscript out of range: "+subscript);
	}
	//Check sanity of bound
	if (bounds[2*subscript] - bounds[2*subscript+1] > __DBL_EPSILON__) crash("ModelTuningParameters","Lower bound " + str(bounds[2*subscript]) + " is larger than upper bound " + str(bounds[2*subscript+1]));
	return bounds[2*subscript];
}

double ModelTuningParameters::getUpperBound(const int subscript) const {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {
		crash("ModelTuningParameters","Upper bound subscript out of range: "+subscript);
	}
	//Check sanity of bound
	if (bounds[2*subscript] - bounds[2*subscript+1] > __DBL_EPSILON__) crash("ModelTuningParameters","Lower bound " + str(bounds[2*subscript]) + " is larger than upper bound " + str(bounds[2*subscript+1]));
	return bounds[2*subscript+1];	
}

unsigned ModelTuningParameters::getLength() const {
	return tuningParameters.size();	
}


void ModelTuningParameters::setTuningParameters(const vector< double > newTParams) {

	tuningParameters = newTParams;	

}

void ModelTuningParameters::setTuningParameters(const string paramString, const int newTParamsLength) {
	vector< double > newTParams(newTParamsLength);
	
	// "Parse" the string
	if (paramString != "") {
		istringstream stream(paramString);	
		for (int i = 0; i < newTParamsLength; i++) {
			if (!stream.good()) crash("ModelTuningParameters","Error while converting string into parameters: "+paramString);
			stream >> newTParams[i];
		}
	}

	ModelTuningParameters::setTuningParameters(newTParams);

}

void ModelTuningParameters::setBounds(const vector< double > newBounds) {

	bounds = newBounds;

}

void ModelTuningParameters::setBounds(const string boundString, const int newBoundsLength) {
	if (boundString != "") {
		vector< double > newBounds(newBoundsLength);
		
		// "Parse" the string
    	istringstream stream (boundString);
    	for (int i = 0; i < newBoundsLength; i++) {
			if (!stream.good()) crash("ModelTuningParameters","Error while converting string into bounds: "+boundString);
        	stream >> newBounds[i];
    	}   
    
    	ModelTuningParameters::setBounds(newBounds);
	}
	else {
		ModelTuningParameters::setBounds(vector< double >(0));	
	}
}

double &ModelTuningParameters::operator[]( int subscript ) {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {crash("ModelTuningParameters","Subscript out of range: "+subscript);}
	return tuningParameters[subscript];
}

const double &ModelTuningParameters::operator[]( int subscript ) const {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {crash("ModelTuningParameters","Subscript out of range: "+subscript);}
	return tuningParameters[subscript];
}

void ModelTuningParameters::setErrorValue(const double newValue) {
	errorValue = newValue;
	errorValueIsValid = true;
}

void ModelTuningParameters::setMOErrorValues(const vector< double > newValues) {
	MOErrorValues = newValues;
}

void ModelTuningParameters::resetErrorValue() {
	errorValueIsValid = false;
}

double ModelTuningParameters::getErrorValue() const {

	if (!errorValueIsValid) {
		crash("ModelTuningParameters","Getting error value which is uninitialized");
	}
	return errorValue;

}


bool ModelTuningParameters::validErrorValue() const {

	return errorValueIsValid;

}	

vector< double > ModelTuningParameters::getMOErrorValues() const {

	if (!errorValueIsValid) {
		crash("ModelTuningParameters","Getting MO error values which are uninitialized");
	}
	return MOErrorValues;

}

double ModelTuningParameters::getMOErrorValue(unsigned index) const {

	if (!errorValueIsValid) {
        crash("ModelTuningParameters","Getting MO error values which are uninitialized");
    }
	if (index >= MOErrorValues.size()) {
        crash("ModelTuningParameters","Getting MO error value with invalid index: "+str(index));
	}
	return MOErrorValues[index];

}
        
unsigned ModelTuningParameters::getNumberOfMOErrorValues() const {

	if (!errorValueIsValid) return 0;
	return MOErrorValues.size();

}

string ModelTuningParameters::toString() const {
	ostringstream o;
	o << "{ ";
	for (unsigned i = 0; i < getLength(); i++) {
   		o << (*this)[i] << " ";		
	}
	o << "}";
	return o.str();
}

void ModelTuningParameters::printOn(OutputChannel & output) const {

	int length = tuningParameters.size();
	output << length; 
	for (int i = 0; i < length; i++) {	
		output << tuningParameters[i];
	}
	int boundsLength = bounds.size();
	output << boundsLength; 
	for (int i = 0; i < boundsLength; i++) {	
		output << bounds[i];
	}
	output << (int)errorValueIsValid;
	output << errorValue;

	output << (unsigned)MOErrorValues.size();
	for (unsigned i = 0; i < MOErrorValues.size(); i++) {
		output << MOErrorValues[i];
	}

}

void ModelTuningParameters::readFrom(InputChannel & input) {

	int length;
	input >> length;
	tuningParameters = vector< double >(length);
	for (int i = 0; i < length; i++) {
		input >> tuningParameters[i];
	}
	
	int boundsLength;
	input >> boundsLength;
	bounds = vector< double >(boundsLength);
	for (int i = 0; i < boundsLength; i++) {
		input >> bounds[i];
	}
	
	int fValid;
	input >> fValid; errorValueIsValid = (bool)fValid;
	input >> errorValue;

	unsigned MOErrorValuesLength;
	input >> MOErrorValuesLength;
	MOErrorValues = vector< double >(MOErrorValuesLength);
	for (unsigned i = 0; i < MOErrorValuesLength; i++) {
		input >> MOErrorValues[i];
	}
	
}
