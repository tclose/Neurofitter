#include "../ModelResults.h"

ModelResults::ModelResults(const int nOTraces) {
	numberOfTraces = nOTraces;
	traces = new DataTrace[numberOfTraces];
	if (traces == NULL) {cerr << endl<< "Error: Unable to allocate memory for ModelResults" << endl;exit(1);}
}

ModelResults::~ModelResults() {
	if (traces != NULL) delete [] traces;
}

ModelResults & ModelResults::operator=(const ModelResults & m) {
	if (this != &m) {
		if (traces != NULL) delete [] traces;
		numberOfTraces = m.numberOfTraces;
		traces = new DataTrace[numberOfTraces];
		for (int i=0; i < m.getLength(); i++) {
			(*this)[i] = m[i];
		}
	}
	return *this;
}

int ModelResults::getLength() const {
	return numberOfTraces;
}
    
DataTrace& ModelResults::operator[] (const int subscript) {
	if (0 <= subscript && subscript < numberOfTraces)	
		{return traces[subscript];}
	else
		{cerr << endl <<"Error: Invalid subscript in ModelResults" << endl;exit(1);}
}

const DataTrace& ModelResults::operator[] (const int subscript) const {
	if (0 <= subscript && subscript < numberOfTraces)
        {return traces[subscript];}
    else
        {cerr << "Error: Invalid subscript in ModelResults";exit(1);}
}
