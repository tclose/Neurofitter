#include "../ModelResults.h"

ModelResults::ModelResults()
	: traces() {}

ModelResults::ModelResults(const int nOTraces) 
	: traces(nOTraces) {}

int ModelResults::getLength() const {
	return traces.size();
}

void ModelResults::printOn(ostream & output) const {
	output << traces.size() << " ";

	for (unsigned int i = 0; i < traces.size(); i++) {
		(traces[i]).printOn(output);
	}
	
	output << " ";
}
    
void ModelResults::readFrom(istream & input) {
	unsigned length;
	input >> length;
	traces = vector< DataTrace >(length);
	
	for (unsigned i = 0; i < length; i++) {
		(traces[i]).readFrom(input);
	}
}
   
DataTrace& ModelResults::operator[] (const int subscript) {
	if (0 <= subscript && subscript < (int)traces.size())	
		{return traces[subscript];}
	else
		{cerr << endl <<"Error: Invalid subscript in ModelResults" << endl;exit(1);}
}

const DataTrace& ModelResults::operator[] (const int subscript) const {
	if (0 <= subscript && subscript < (int)traces.size())
        {return traces[subscript];}
    else
        {cerr << "Error: Invalid subscript in ModelResults";exit(1);}
}
