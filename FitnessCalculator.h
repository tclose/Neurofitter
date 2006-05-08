#ifndef FITNESSCALCULATOR_H
#define FITNESSCALCULATOR_H

#include <vector>

#include "ModelTuningParameters.h"
#include "ModelInterface.h"

using namespace std;

class FitnessCalculator {

	public:
		FitnessCalculator(): model(NULL), exportFile(""), fitnessHistory() {};
		virtual double calculateFitness(ModelTuningParameters & params) = 0;
		virtual void enableFileExport(const string fileName) = 0;
		virtual void disableFileExport() = 0;
		virtual string getExportFile() const = 0;
		virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory() = 0;
		virtual ~FitnessCalculator() {};


	protected:
		const ModelInterface * model;
		string exportFile;
		vector< pair< ModelTuningParameters, double > > fitnessHistory;
};

#endif
