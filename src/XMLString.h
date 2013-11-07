/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_XMLSTRING_H
#define NEUROFITTER_XMLSTRING_H

#include <vector>
#include <string>
#include <iostream>
#include <libxml/parser.h>

using namespace std;

/// Class that provides an interface with XML formatting
class XMLString {

	public:
		XMLString() : sourceString("") {}; ///< Default constructor
		XMLString(const string); ///< Constructor that uses an initializing string
		~XMLString(); ///< Destructor

		string toString() const; ///< Returns a string representation of the XML
		XMLString getSubXMLString(const string) const; ///< Get a subset of the XML data with a certain tag
		string getSubString(const string) const; ///< Get a subset of the XML with a certain tag as a string
		vector<string> getSubNames() const; ///< Get all the names of the subset tags

		string getContent() const; ///< Return the string representing the XML
		string getName() const; ///< Return the name of the root element

		static string removeXMLComments(string); ///< Remove the comments (can be nested) from an XML string

	private:
		string sourceString; ///< String containing the XML
};

#endif
