#include "../ModelTuningParameters.h"

///todo clean up from non-vector era

//todo make this one private to prevent ModelTuningParameters with length 0
ModelTuningParameters::ModelTuningParameters() : 
	tuningParameters(), bounds(), fitnessValue(-1) {}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) 
	: tuningParameters(vector< double >(newTParamsLength)), bounds(), fitnessValue(-1) {
}

ModelTuningParameters::ModelTuningParameters(const vector< double > newTParams, const int newTParamsLength, const vector < double > newBounds) : 
	tuningParameters(), bounds(), fitnessValue(-1) {

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength);
}

ModelTuningParameters::ModelTuningParameters(const string paramString, const int newTParamsLength, const string newBounds) : 
	tuningParameters(), bounds(), fitnessValue(-1) {

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

	if (paramString != "") {
		istringstream stream (paramString);	
		for (int i = 0; i < newTParamsLength; i++) {
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

    	istringstream stream (boundString);
    	for (int i = 0; i < newBoundsLength; i++) {
        	stream >> newBounds[i];
    	}   
    
    	ModelTuningParameters::setBounds(newBounds, newBoundsLength);
	}
	else {
		ModelTuningParameters::setBounds(NULL, 0);	
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
}

double ModelTuningParameters::getFitnessValue() const {

	if (fitnessValue < 0) {
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

string ModelTuningParameters::serialize() const {

	string serialString;
	
	
	
	return serialString;
}

void ModelTuningParameters::printOn(ostream output) const {

	output << tuningParameters.size() << " "; 
	for (int i = 0; i < (int)tuningParameters.size(); i++) {	
		output << tuningParameters[i] << " ";
	}
	output << bounds.size() << " "; 
	for (int i = 0; i < (int)bounds.size(); i++) {	
		output << bounds[i] << " ";
	}
	output << fitnessValue << " ";

}

void ModelTuningParameters::readFrom(istream input) {

	int length;
	input >> length;
	vector< double > tuningParameters(length);
	for (int i = 0; i < (int)tuningParameters.size(); i++) {
		input >> tuningParameters[i];
	}
	
	int boundsLength;
	input >> boundsLength;
	vector< double > bounds(boundsLength);
	for (int i = 0; i < (int)bounds.size(); i++) {
		input >> bounds[i];
	}
	
}
