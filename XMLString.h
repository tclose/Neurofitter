#ifndef XMLSTRING_H
#define XMLSTRING_H

#include <vector>
#include <string>
#include <iostream>
#include <libxml/parser.h>

#include "Tools.h"

using namespace std;

class XMLString {

	public:
		XMLString() : sourceString("") {};
		XMLString(const string);
		~XMLString();

		string toString() const;
		XMLString getSubXMLString(const string) const;
		string getSubString(const string) const;
		vector<string> getSubNames() const;

		string getContent() const;
		string getName() const;

	private:
		string sourceString;
};

#endif
