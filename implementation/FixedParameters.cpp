#include "../Tools.h"

#include "../FixedParameters.h"
    
FixedParameters::FixedParameters(string parameters, bool isGlobal) {
	readFromXML(parameters,isGlobal);
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
        	returnString += "<" + i->first + ">" + (i->second).first + "</" + i->first + ">";
		}
    }
	return returnString;	
}

string& FixedParameters::operator[](const string index) {
	map<const string, pair< string, bool > >::iterator i = params.find(index);
	if (i == params.end()) crash("FixedParameters","Unable to find key: "+index);
	return (i->second).first;
}

const string& FixedParameters::operator[](const string index) const {
	map<const string, pair< string, bool > >::const_iterator i = params.find(index);
	if (i == params.end()) crash("FixedParameters","Unable to find key: "+index);
	return (i->second).first;
}

void FixedParameters::readFromXML(string parameters, bool global) {
	XMLString xml("<xmlroot>"+parameters+"</xmlroot>");
	vector<string> names = xml.getSubNames();
	vector<string>::iterator i;
	
	for (i = names.begin(); i != names.end(); i++) {
		params[*i].first = xml.getSubString(*i);
		params[*i].second = global;
	}
}

bool FixedParameters::parameterExists(const string index) const {
	map<const string, pair< string, bool > >::const_iterator i = params.find(index);
	return !(i == params.end());
}

void FixedParameters::addParameter(const string index, const string value, const bool isGlobal) {
	params[index].first = value;
	params[index].second = isGlobal;
}

void FixedParameters::removeParameter(const string index) {
	params.erase(index);
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



