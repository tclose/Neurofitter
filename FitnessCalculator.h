#ifndef FITNESSCALCULATOR_H
#define FITNESSCALCULATOR_H

#include "ModelTuningParameters.h"
#include "ModelInterface.h"

class FitnessCalculator {

	public:
		FitnessCalculator(): model(NULL), exportFile("") {};
		virtual double calculateFitness(const ModelTuningParameters & params) = 0;
	//	virtual void readFixedParameters(const string);
		virtual void enableFileExport(const string fileName) = 0;
		virtual void disableFileExport() = 0;
		virtual string getExportFile() const = 0;
		virtual ~FitnessCalculator() {};


	protected:
		const ModelInterface * model;
		string exportFile;
};

#endif
