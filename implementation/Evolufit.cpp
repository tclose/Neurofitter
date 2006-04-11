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

extern void setNOMADFitnessCalculator(FitnessCalculator*);

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

	////
	//	Initialize
	////
	
//	WernerExperimentInterface experiment = WernerExperimentInterface();	
/*
	double testSet1 [] = {500,15,15000,50};
	ModelTuningParameters modelparams1(testSet1,4);
*/
/*	double initParamsArray [] = {6000.0, 6.0, 600.0,1000.0};
	//double bounds [] = {70000.0, 80000.0, 1.0, 20.0, 100.0, 200.0, 4000.0, 6000.0};
	double bounds [] = {1.0, 100000.0, 1.0, 20.0, 1.0, 1000.0, 1.0, 10000.0};

	ModelTuningParameters initParams(initParamsArray,4,bounds);


	WernerModelInterface model = WernerModelInterface(); 
*/
/*
	test = model.runModel(modelparams);

	for (int i = 0; i < test.getLength(); i++) {
		cout 	<< test[i].getName() << endl;
		cout 	<< "StartTime: " << str(test[i].getStartTime()) << " StopTime: " << str(test[i].getStopTime()) << endl; 
		cout	<< "Weight: " << str(test[i].getWeight()) << " Sampling Frequency: " << str(test[i].getSamplingFrequency());
		for (int j = 0; j < test[i].getLength(); j++) {
			if (j > 10) break;
			cout << " "<< test[i][j];
		}
		cout << endl;
	}
	

*/  
/*
	PabloFitnessCalculator fitcal(&model, &experiment); 
*/

/*
	cout << endl << "Fitness value: " << fitcal.calculateFitness(modelparams1) << endl;

	cout << endl << "Fitness value: " << fitcal.calculateFitness(modelparams2) << endl;

*/	
/*	EOFitterInterface eo = EOFitterInterface();

	eo.runFitter(&fitcal, &initParams, 5);
*/
/*	NOMADFitterInterface nomad = NOMADFitterInterface();
	setNOMADFitnessCalculator(&fitcal);
	nomad.runFitter(&fitcal, &initParams, 5);
*/

	////
	//	Run EO
	////
		

	////
	// Run NOMAD
	////


	////
	// Finish up
	////

	cout << endl << "And we have touchdown" << endl;

	return 0;
}
