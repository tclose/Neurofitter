#include "../XMLString.h"

XMLString::XMLString(const string source)
	:sourceString(source) {
	//if (xml == NULL) {
	//	cerr << "Unable to parse string in XMLString: " << source << endl;
	//	exit(1);
	//}
}

XMLString::~XMLString() {
	//if (xml != NULL) xmlFreeDoc(xml);;
}

string XMLString::toString() const {
	return sourceString;
}
        
string XMLString::getSubString(const string subName) const {
	xmlDocPtr xml(xmlParseMemory(sourceString.c_str(), sourceString.length()+1));
	xmlNodePtr cur = (xmlDocGetRootElement(xml))->xmlChildrenNode;
    while (cur != NULL) {
		if (string((char*)cur->name) == subName) {
			xmlDocPtr tmp = xmlCopyDoc(xml, 1);
			xmlDocSetRootElement(tmp, cur);
			xmlChar * children;
    		int size;
			xmlDocDumpMemory(tmp, &children, &size);
			xmlFree(tmp);
			string subString((char*)children);
        	return subString;
		}	
        cur = cur->next;
    }
	cerr << "Unable to find name of sub in XMLString: " << subName << endl; exit(1);
	return ""; 
}        

XMLString XMLString::getSubXMLString(const string subName) const {
	return XMLString(getSubString(subName));
}

string XMLString::getContent() const {
	xmlDocPtr xml(xmlParseMemory(sourceString.c_str(), sourceString.length()+1));
	char * content = (char*)xmlNodeListGetString(xml, (xmlDocGetRootElement(xml))->xmlChildrenNode, 1);
	string contentString(content);
	xmlFree(content);
	return contentString;
}

string XMLString::getName() const {
	xmlDocPtr xml(xmlParseMemory(sourceString.c_str(), sourceString.length()+1));
	return string((char*)xmlDocGetRootElement(xml)->name);
}

vector<string> XMLString::getSubNames() const{
	xmlDocPtr xml(xmlParseMemory(sourceString.c_str(), sourceString.length()+1));
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
	return names;
}
