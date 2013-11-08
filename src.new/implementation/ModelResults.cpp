/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

#include "../ModelResults.h"

ModelResults::ModelResults()
	: traces() {}

ModelResults::ModelResults(const int nOTraces) 
	: traces(nOTraces) {}

int ModelResults::getLength() const {
	return traces.size();
}

void ModelResults::printOn(OutputChannel & output) const {
	int length = (int)traces.size();
	output << length;

	for (unsigned int i = 0; i < traces.size(); i++) {
		(traces[i]).printOn(output);
	}
}
    
void ModelResults::readFrom(InputChannel & input) {
 	int length;
	input >> length;
	traces = vector< DataTrace >(length);
	
	for (int i = 0; i < length; i++) {
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
