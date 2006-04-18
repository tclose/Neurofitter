/*
* Algorithm
*
*
* read evolufit parameters
*
* read starting points
*
* create fitness calculator
* 
* start EO
*	give parameters
*	give starting points
*	give fitness calculator
* get EO information
* get EO result
*
*
* start NOMAD
*	give parameters
*	give starting point from EO
*	give fitness calculator
* get NOMAD information
* get NOMAD result
*
* write evolufit results / fitting info
*
*/

#include <iostream>
#include <libxml2/libxml/parser.h>

#include "../Evolufit.h"

using namespace std;

static void
print_element_names(xmlNode * a_node)
{
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }

        print_element_names(cur_node->children);
    }
}

int main () {

	cout << "Houston, we have liftoff...\n" ;
/*
	string docname = "parameters.xml";

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname.c_str());
	
	if (doc == NULL ) {
		cerr << "Document not parsed successfully. \n";
		return 1;
	}

	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		cerr << "empty document\n";
		xmlFreeDoc(doc);
		return 1;
	}

	while (cur != NULL) {	
		cout << string((char*)xmlNodeListGetRawString(doc, cur->xmlChildrenNode, 0));
		cur = cur->next;
	}


    print_element_names(xmlDocGetRootElement(doc));

	xmlChar * test;
	int size;

	xmlDocDumpMemory(doc, &test, &size);

	cout << string((char*)test);
*/
	////
	//	Initialize
	////
	
	double expParams [] = {2800.0, 11200.0};

	double startPointArray [] = {1.0, 1.0};
	double bounds [] = {1.0, 10000.0, 1.0, 50000.0};


	ModelTuningParameters startPoint(startPointArray,2,bounds);

	double meshArray [] = {10, 10};
	ModelTuningParameters mesh(meshArray,2,bounds);

	WernerModelInterface model = WernerModelInterface(); 
	WernerExperimentInterface experiment = WernerExperimentInterface(ModelTuningParameters(expParams,2,bounds));	

	PabloFitnessCalculator fitcal(&model,&experiment);

	fitcal.enableFileExport("fitness.dat");

	//MeshFitterInterface fitter = MeshFitterInterface(&fitcal);
	//fitter.runFitter(&mesh, 1550);


	SwarmFitterInterface fitter = SwarmFitterInterface(&fitcal,5,10);
	//PabloFitterInterface fitter(&model, &experiment);
	//NOMADFitterInterface fitter(&fitcal, startPoint.getLength());
	//EOFitterInterface fitter(&fitcal);

	fitter.runFitter(&startPoint, 1550);

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
