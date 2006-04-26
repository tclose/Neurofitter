#ifndef FIXEDPARAMETERS_H
#define FIXEDPARAMETERS_H

#include <map>
#include <string>

#include "XMLString.h"
#include "Tools.h"


///todo add this to every header file, and also include stdlib
using namespace std;

class FixedParameters {

	public:
		FixedParameters() {};
		FixedParameters(string parameters);
		FixedParameters(string parameters, string globalparameters);

		string& operator[](const string index);
		const string&operator[](const string index) const;

		string getGlobals() const;

		void readFromXML(string parameters, bool global);

		void setGlobal(string);
		void notGlobal(string);
		bool getGlobal(string);

		string toString() const;	
	private:
		map< string, pair< string,bool > > params;

};

#endif
