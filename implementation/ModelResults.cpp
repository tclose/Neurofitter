#include "../ModelResults.h"

ModelResults::ModelResults()
	: traces(), numberOfTraces(0) {}

ModelResults::ModelResults(const int nOTraces) 
	: traces(nOTraces), numberOfTraces(nOTraces) {}

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
