#include "../ModelTuningParameters.h"

///todo clean up from non-vector era

//todo make this one private to prevent ModelTuningParameters with length 0
ModelTuningParameters::ModelTuningParameters() : 
	tuningParameters(), bounds(), tuningParametersLength(0) {}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) 
	: tuningParameters(vector< double >(newTParamsLength)), bounds(), tuningParametersLength(newTParamsLength) {
}

ModelTuningParameters::ModelTuningParameters(const vector< double > newTParams, const int newTParamsLength, const vector < double > newBounds) : 
	tuningParameters(), bounds() {

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength);
}

ModelTuningParameters::ModelTuningParameters(const string paramString, const int newTParamsLength, const string newBounds) : 
	tuningParameters(), bounds() {

	ModelTuningParameters::setTuningParameters(paramString, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
}

double ModelTuningParameters::getLowerBound(const int subscript) const {
	if (0 <= subscript && subscript < tuningParametersLength) {
		return bounds[2*subscript];
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

double ModelTuningParameters::getUpperBound(const int subscript) const {
	if (0 <= subscript && subscript < tuningParametersLength) {
		return bounds[2*subscript+1];
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

void ModelTuningParameters::setLowerBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < tuningParametersLength) {
		bounds[2*subscript] = newBound;
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

void ModelTuningParameters::setUpperBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < tuningParametersLength) {
		bounds[2*subscript+1] = newBound;
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

int ModelTuningParameters::getLength() const {
	return tuningParametersLength;	
}


void ModelTuningParameters::setTuningParameters(const vector< double > newTParams, const int newTParamsLength) {

	tuningParameters = newTParams;	
	tuningParametersLength = newTParamsLength;	
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
	if (0 <= subscript && subscript < tuningParametersLength) {return tuningParameters[subscript];}	
	else {cerr << "Subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

const double &ModelTuningParameters::operator[]( int subscript ) const {
	if (0 <= subscript && subscript < tuningParametersLength) {return tuningParameters[subscript];}	
	else {cerr << "Subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}

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
