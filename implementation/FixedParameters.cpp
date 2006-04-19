#include "../FixedParameters.h"
    
FixedParameters::FixedParameters(XMLString parameters) {
	readFromXML(parameters);
}

FixedParameters::FixedParameters(XMLString parameters, FixedParameters globalParameters) {
	params = globalParameters.params;
	readFromXML(parameters);
}

string& FixedParameters::operator[](const string index) {
	return params[index];
}

const string FixedParameters::operator[](const string index) const {
	map<const string, string>::const_iterator i = params.find(index);
	return i->second;
}

void FixedParameters::readFromXML(XMLString parameters) {
	vector<string> names = parameters.getSubNames();
	vector<string>::iterator i;
	
	for (i = names.begin(); i != names.end(); i++) {
		params[*i] = parameters.getSubString(*i);
	}
}
