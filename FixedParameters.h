#ifndef FIXEDPARAMETERS_H
#define FIXEDPARAMETERS_H

#include <map>
#include <string>

#include "XMLString.h"

///todo add this to every header file, and also include stdlib
using namespace std;

class FixedParameters {

	public:
		FixedParameters() {};
		FixedParameters(XMLString parameters);
		FixedParameters(XMLString parameters, FixedParameters globalparameters);

		string& operator[](const string index);
		const string operator[](const string index) const;

		void readFromXML(XMLString parameters);
	
	private:
		map< string, string> params;

};

#endif
