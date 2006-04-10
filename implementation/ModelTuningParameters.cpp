#include "../ModelTuningParameters.h"

//todo make this one private to prevent ModelTuningParameters with length 0
ModelTuningParameters::ModelTuningParameters() {
	tuningParameters = NULL;
	tuningParametersLength = 0;	
}

ModelTuningParameters::~ModelTuningParameters() {
	if (tuningParameters != NULL) delete [] tuningParameters;
}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) {
	tuningParameters = new double [newTParamsLength];
	if (tuningParameters==NULL) {cerr << "Unable to allocate memory in ModelTuningParameters"<<endl;exit(1);}
	tuningParametersLength = newTParamsLength;
}

ModelTuningParameters::ModelTuningParameters(const double * newTParams, const int newTParamsLength, const double * newBounds) {
	tuningParameters = NULL;
	bounds = NULL;

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);
	if (newBounds != NULL) {
		ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
	} else {
		cerr << endl << "Warning: Creating ModelTuningParameters without bounds" << endl;	
	}


}

double * ModelTuningParameters::getTuningParameters() const {
	return tuningParameters;	
}


double ModelTuningParameters::getLowerBound(const int subscript) const {
	if (0 <= subscript && subscript < tuningParametersLength && bounds != NULL) {
		return bounds[2*subscript];
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

double ModelTuningParameters::getUpperBound(const int subscript) const {
	if (0 <= subscript && subscript < tuningParametersLength && bounds != NULL) {
		return bounds[2*subscript+1];
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

void ModelTuningParameters::setLowerBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < tuningParametersLength && bounds != NULL) {
		bounds[2*subscript] = newBound;
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

void ModelTuningParameters::setUpperBound(const int subscript, const double newBound) {
	if (0 <= subscript && subscript < tuningParametersLength && bounds != NULL) {
		bounds[2*subscript+1] = newBound;
	}	
	else {cerr << "Bounds subscript out of range in ModelTuningParameters: "<<subscript<<endl;exit(1);}
}

double * ModelTuningParameters::getBounds() const {
	return bounds;	
}

ModelTuningParameters & ModelTuningParameters::operator=(const ModelTuningParameters & t) {
	if (this != &t) {
			setTuningParameters(t.tuningParameters, t.tuningParametersLength);
			setBounds(t.bounds, 2*t.tuningParametersLength);
	}
	return *this;
}

int ModelTuningParameters::getLength() const {
	return tuningParametersLength;	
}


void ModelTuningParameters::setTuningParameters(const double * newTParams, const int newTParamsLength) {
	if (tuningParameters != NULL) {delete [] tuningParameters;}

	tuningParameters = new double [newTParamsLength];
	if (tuningParameters==NULL) {cerr << "Unable to allocate memory in ModelTuningParameters"<<endl;exit(1);}
	tuningParametersLength = newTParamsLength;	
	for (int i = 0; i < newTParamsLength; i++) {
		tuningParameters[i] = newTParams[i];
	}
}

void ModelTuningParameters::setBounds(const double * newBounds, const int newBoundsLength) {
	if (bounds != NULL) {delete [] bounds;}

	bounds = new double [newBoundsLength];
	if (bounds==NULL) {cerr << "Unable to allocate memory in ModelTuningParameters"<<endl;exit(1);}
	for (int i = 0; i < newBoundsLength; i++) {
		bounds[i] = newBounds[i];
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
	o << "{";
	for (int i = 0; i < getLength(); i++) {
   		o << (*this)[i] << ", ";		
	}
	o << "}";
	return o.str();
}
