#include "../Tools.h"

#include "../XMLString.h"

///todo check all xml objects for leaks

/// Constructor that uses an initializing string
XMLString::XMLString(const string source)
	:sourceString(source) {
}

/// Destructor
XMLString::~XMLString() {
}

/// Returns a string representation of the XML
string XMLString::toString() const {
	return sourceString;
}
   
/// Get a subset of the XML data with a certain tag     
string XMLString::getSubString(const string subName) const {
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	xmlNodePtr cur = (xmlDocGetRootElement(xml))->xmlChildrenNode;

	string returnString = "";

    while (cur != NULL) {
		if (string((char*)cur->name) == subName) {
			xmlNodePtr child = cur->xmlChildrenNode;
			while (child != NULL) {
				xmlBufferPtr buf = xmlBufferCreate();
				xmlNodeDump(buf,xml,child,100,0);
				returnString += string((char*)buf->content,buf->use);
				xmlBufferFree(buf);
				child = child->next;
			}
		}	
        cur = cur->next;
    }
	if (returnString == "") crash("XMLString","Unable to find content of sub in XMLString: " + subName);
	xmlFreeDoc(xml);
	//xmlFree(cur);
	//xmlCleanupParser();
	return returnString; 
}        

/// Get a subset of the XML with a certain tag as a string
XMLString XMLString::getSubXMLString(const string subName) const {
	return XMLString(getSubString(subName));
}

/// Return the string representing the XML
string XMLString::getContent() const {
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	char * content = (char*)xmlNodeListGetString(xml, (xmlDocGetRootElement(xml))->xmlChildrenNode, 1);
	string contentString(content);
	//xmlFree(content);
	xmlFreeDoc(xml);
	//xmlCleanupParser();

	return contentString;
}

/// Return the name of the root element
string XMLString::getName() const {
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	string returnString((char*)xmlDocGetRootElement(xml)->name);
	xmlFreeDoc(xml);
	//xmlCleanupParser();

	return returnString;
}

/// Return the name of the root element
vector<string> XMLString::getSubNames() const{
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	vector<string> names;
	
	if (xml != NULL) {
		xmlNode * elements = xmlDocGetRootElement(xml)->children;
    	xmlNode * cur_node = NULL;

    	for (cur_node = elements; cur_node; cur_node = cur_node->next) {
        	if (cur_node->type == XML_ELEMENT_NODE) {
            	names.push_back(string((char*)(cur_node->name)));
        	}	
    	}
	}
	xmlFreeDoc(xml);
	//xmlCleanupParser();

	return names;
}

/// Removes comments from the XML file, not very efficient but fast enough
string XMLString::removeXMLComments(string input) {

	string output = input;
	unsigned int start, stop;
	
	if ((start = output.find("<!--",0)) != string::npos) {
		output = output.substr(0,start)+removeXMLComments(output.substr(start+4));
		stop = output.find("-->",start);
		if (stop == string::npos) crash("XMLString","Mismatch in XML comments");
		output = output.substr(0,start)+output.substr(stop+3);
	}

	return output;

}
