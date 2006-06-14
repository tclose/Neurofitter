#ifndef FIXEDPARAMETERS_H
#define FIXEDPARAMETERS_H

#include <map>
#include <string>

using namespace std;

#include "XMLString.h"
#include "Tools.h"

using namespace std;

class FixedParameters {

	public:
		FixedParameters() {};
		FixedParameters(string parameters, bool isGlobal = false);
		FixedParameters(string parameters, string globalparameters);

		string& operator[](const string index);
		const string&operator[](const string index) const;

		string getGlobals() const;

		void readFromXML(string parameters, bool global);

		void addParameter(const string index, const string value, const bool isGlobal = false);

		void setGlobal(string);
		void notGlobal(string);
		bool getGlobal(string);

		string getProp(string);

		string toString() const;	
	private:
		map< string, pair< string,bool > > params;

};

#endif
