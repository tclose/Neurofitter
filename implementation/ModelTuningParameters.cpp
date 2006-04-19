#include "../ModelTuningParameters.h"

//todo make this one private to prevent ModelTuningParameters with length 0
ModelTuningParameters::ModelTuningParameters() : 
	tuningParameters(NULL), bounds(NULL), tuningParametersLength(0) {}

ModelTuningParameters::~ModelTuningParameters() {
	if (tuningParameters != NULL) delete [] tuningParameters;
 	if (bounds != NULL) delete [] bounds;
}

ModelTuningParameters::ModelTuningParameters(const int newTParamsLength) : 
	tuningParameters(new double [newTParamsLength]), bounds(NULL), tuningParametersLength(newTParamsLength) {
	if (tuningParameters==NULL) {cerr << "Unable to allocate memory in ModelTuningParameters"<<endl;exit(1);}
}

ModelTuningParameters::ModelTuningParameters(const double * newTParams, const int newTParamsLength, const double * newBounds) : 
	tuningParameters(NULL), bounds(NULL) {

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength);
}

ModelTuningParameters::ModelTuningParameters(const string paramString, const int newTParamsLength, const string newBounds) : 
	tuningParameters(NULL), bounds(NULL) {

	ModelTuningParameters::setTuningParameters(paramString, newTParamsLength);
	ModelTuningParameters::setBounds(newBounds, 2*newTParamsLength); 
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

void ModelTuningParameters::setTuningParameters(const string paramString, const int newTParamsLength) {
	double * newTParams = new double [newTParamsLength];

	istringstream stream (paramString);	
	for (int i = 0; i < newTParamsLength; i++) {
		stream >> newTParams[i];
	}

	ModelTuningParameters::setTuningParameters(newTParams, newTParamsLength);

	delete [] newTParams;
}

void ModelTuningParameters::setBounds(const double * newBounds, const int newBoundsLength) {
	if (bounds != NULL) {delete [] bounds;}

	if (newBounds != NULL) {
		bounds = new double [newBoundsLength];
		if (bounds==NULL) {cerr << "Unable to allocate memory in ModelTuningParameters"<<endl;exit(1);}
		for (int i = 0; i < newBoundsLength; i++) {
			bounds[i] = newBounds[i];
		}
	}
	else {
		bounds = NULL;
		//cerr << endl << "Warning: Creating ModelTuningParameters without bounds" << endl;
	}
}

void ModelTuningParameters::setBounds(const string boundString, const int newBoundsLength) {
	double * newBounds = new double [newBoundsLength];

    istringstream stream (boundString);
    for (int i = 0; i < newBoundsLength; i++) {
        stream >> newBounds[i];
    }   
    
    ModelTuningParameters::setBounds(newBounds, newBoundsLength);

    delete [] newBounds;


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
