#include "../FixedParameters.h"
    
FixedParameters::FixedParameters(string parameters) {
	readFromXML(parameters,false);
}

FixedParameters::FixedParameters(string parameters, string globalParameters) {
	readFromXML(globalParameters,true);
	readFromXML(parameters,false);
}

string FixedParameters::getGlobals() const {
	string returnString = "";
	map<const string, pair< string, bool > >::const_iterator i;
    for (i=params.begin(); i !=params.end(); i++) {
		if ((i->second).second) {
        	returnString += "<" + i->first + ">\n" + (i->second).first + "</" + i->first + ">\n";
		}
    }
	return returnString;	
}

string& FixedParameters::operator[](const string index) {
	return params[index].first;
}

const string& FixedParameters::operator[](const string index) const {
	map<const string, pair< string, bool > >::const_iterator i = params.find(index);
	return (i->second).first;
}

void FixedParameters::readFromXML(string parameters, bool global) {
	XMLString xml("<xmlroot>\n"+parameters+"</xmlroot>");
	vector<string> names = xml.getSubNames();
	vector<string>::iterator i;
	
	for (i = names.begin(); i != names.end(); i++) {
		params[*i].first = xml.getSubString(*i);
		params[*i].second = global;
	}
}

string FixedParameters::toString() const {
	string returnString;
	map<const string, pair< string, bool > >::const_iterator i;
	for (i=params.begin(); i !=params.end(); i++) {
		returnString += i->first 
					+ ((i->second).second ? "(Global)":"")  
					+ ": "+(i->second).first+"\n";
	}
	return returnString;
}

void FixedParameters::setGlobal(string index) {
	params[index].second = true;
}

void FixedParameters::notGlobal(string index) {
	params[index].second = false;
}

bool FixedParameters::getGlobal(string index) {
	return params[index].second;
}


