#include "../ModelTuningParameters.h"

ModelTuningParameters::ModelTuningParameters() : 
	tuningParameters(), bounds(), fitnessIsValid(false) {}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) 
	: tuningParameters(vector< double >(newTParamsLength)), bounds(), fitnessIsValid(false) {
}

ModelTuningParameters::ModelTuningParameters(const vector< double > newTParams, const int newTParamsLength, const vector < double > newBounds) : 
	tuningParameters(), bounds(), fitnessIsValid(false) {

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength);
}

ModelTuningParameters::ModelTuningParameters(const string paramString, const int newTParamsLength, const string newBounds) : 
	tuningParameters(), bounds(), fitnessIsValid(false) {

	ModelTuningParameters::setTuningParameters(paramString, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
}

double ModelTuningParameters::getLowerBound(const int subscript) const {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {
		crash("ModelTuningParameters","Lower bound subscript out of range: "+subscript);
	}
	return bounds[2*subscript];
}

double ModelTuningParameters::getUpperBound(const int subscript) const {
	if (subscript < 0 || subscript >= (int)tuningParameters.size()) {
		crash("ModelTuningParameters","Upper bound subscript out of range: "+subscript);
	}
	return bounds[2*subscript+1];	
}

void ModelTuningParameters::setLowerBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < (int)tuningParameters.size()) {
		bounds[2*subscript] = newBound;
	}	
	else crash("ModelTuningParameters","Lower bound subscript out of range: "+subscript);
}

void ModelTuningParameters::setUpperBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < (int)tuningParameters.size()) {
		bounds[2*subscript+1] = newBound;
	}	
	else crash("ModelTuningParameters","Upper bound subscript out of range: "+subscript);
}

int ModelTuningParameters::getLength() const {
	return tuningParameters.size();	
}


void ModelTuningParameters::setTuningParameters(const vector< double > newTParams, const int newTParamsLength) {

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

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);

}

void ModelTuningParameters::setBounds(const vector< double > newBounds, const int newBoundsLength) {

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
    
    	ModelTuningParameters::setBounds(newBounds, newBoundsLength);
	}
	else {
		ModelTuningParameters::setBounds(vector< double >(0), 0);	
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

void ModelTuningParameters::setFitnessValue(const double newValue) {
	fitnessValue = newValue;
	fitnessIsValid = true;
}

double ModelTuningParameters::getFitnessValue() const {

	if (!fitnessIsValid) {
		crash("ModelTuningParameters","Getting fitness value which is uninitialized");
	}
	return fitnessValue;

}

string ModelTuningParameters::toString() const {
	ostringstream o;
	o << "{ ";
	for (int i = 0; i < getLength(); i++) {
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
	output << (int)fitnessIsValid;
	output << fitnessValue;

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
	input >> fValid; fitnessIsValid = (bool)fValid;
	input >> fitnessValue;
	
}
