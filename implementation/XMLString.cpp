#include "../XMLString.h"

///todo check all xml objects for leaks

XMLString::XMLString(const string source)
	:sourceString(source) {
}

XMLString::~XMLString() {
}

string XMLString::toString() const {
	return sourceString;
}
        
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
	if (returnString == "") {cerr << "Unable to find content of sub in XMLString: " << subName << endl; exit(1);}
	xmlFreeDoc(xml);
	xmlFree(cur);
	xmlCleanupParser();
	return returnString; 
}        

XMLString XMLString::getSubXMLString(const string subName) const {
	return XMLString(getSubString(subName));
}

string XMLString::getContent() const {
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	char * content = (char*)xmlNodeListGetString(xml, (xmlDocGetRootElement(xml))->xmlChildrenNode, 1);
	string contentString(content);
	xmlFree(content);
	xmlFreeDoc(xml);
	xmlCleanupParser();

	return contentString;
}

string XMLString::getName() const {
	xmlDocPtr xml = xmlParseMemory(sourceString.c_str(), sourceString.length()+1);
	string returnString((char*)xmlDocGetRootElement(xml)->name);
	xmlFreeDoc(xml);
	xmlCleanupParser();

	return returnString;
}

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
	xmlCleanupParser();

	return names;
}

