/*
Revision of last commit: $Rev: 222 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:03:41 +0900 (Thu, 04 Oct 2007) $
*/

#ifndef NEUROFITTER_ERRORVALUECALCULATOR_H
#define NEUROFITTER_ERRORVALUECALCULATOR_H

#include <vector>

#include "ModelTuningParameters.h"
#include "ModelInterface.h"

using namespace std;

class ErrorValueCalculator {

	public:
		ErrorValueCalculator(): model(NULL), exportFile(""), errorValueHistory(), numberOfEvaluations(0), numberOfGenerations(0) {};
		ErrorValueCalculator(ModelInterface * interface): model(interface), exportFile(""), errorValueHistory(), numberOfEvaluations(0), numberOfGenerations(0) {};
		virtual void calculateErrorValue(ModelTuningParameters & params) = 0;
		virtual void calculateParallelErrorValue(vector< ModelTuningParameters > & params) = 0;
		virtual void enableFileExport(const string fileName) = 0;
		virtual void disableFileExport() = 0;
		string getExportFile() const {return exportFile;};
		virtual vector< pair< ModelTuningParameters, double > > getErrorValueHistory() = 0;
		int getNumberOfEvalutions() const {return numberOfEvaluations;};
		int getNumberOfGenerations() const {return numberOfGenerations;};

		void putOnStack();
		

		virtual ~ErrorValueCalculator() {};


	protected:
		ModelInterface * model;
		string exportFile;
		vector< pair< ModelTuningParameters, double > > errorValueHistory;
		int numberOfEvaluations;
		int numberOfGenerations;
};

#endif
