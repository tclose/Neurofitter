#ifndef FITNESSCALCULATOR_H
#define FITNESSCALCULATOR_H

#include <vector>

#include "ModelTuningParameters.h"
#include "ModelInterface.h"

using namespace std;

class FitnessCalculator {

	public:
		FitnessCalculator(): model(NULL), exportFile(""), fitnessHistory(), numberOfEvaluations(0), numberOfGenerations(0) {};
		FitnessCalculator(ModelInterface * interface): model(interface), exportFile(""), fitnessHistory(), numberOfEvaluations(0), numberOfGenerations(0) {};
		virtual double calculateFitness(ModelTuningParameters & params) = 0;
		virtual vector< double > calculateParallelFitness(vector< ModelTuningParameters > & params) = 0;
		virtual void enableFileExport(const string fileName) = 0;
		virtual void disableFileExport() = 0;
		string getExportFile() const {return exportFile;};
		virtual vector< pair< ModelTuningParameters, double > > getFitnessHistory() = 0;
		int getNumberOfEvalutions() const {return numberOfEvaluations;};
		int getNumberOfGenerations() const {return numberOfGenerations;};
		virtual ~FitnessCalculator() {};


	protected:
		ModelInterface * model;
		string exportFile;
		vector< pair< ModelTuningParameters, double > > fitnessHistory;
		int numberOfEvaluations;
		int numberOfGenerations;
};

#endif
