#include "../FixedParameters.h"
    
FixedParameters::FixedParameters(string parameters) {
	readFromXML(parameters);
}

FixedParameters::FixedParameters(string parameters, FixedParameters globalParameters) {
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

void FixedParameters::readFromXML(string parameters) {
	//XMLString xml(parameters);
	XMLString xml("<xmlroot>\n"+parameters+"</xmlroot>");
	vector<string> names = xml.getSubNames();
	vector<string>::iterator i;
	
	for (i = names.begin(); i != names.end(); i++) {
		params[*i] = xml.getSubString(*i);
	}
}

string FixedParameters::toString() const {
	string returnString;
	map<const string, string>::const_iterator i;
	for (i=params.begin(); i !=params.end(); i++) {
		returnString += i->first + ": "+i->second+"\n";
	}
	return returnString;
}
