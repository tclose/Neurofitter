#include "../PabloFitnessCalculator.h"

static int numberOfEvaluations = 0;

PabloFitnessCalculator::PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment) {
	model = interface;
	ModelResults expData = experiment->getData();
	
	expVdVdtMatrices = new PabloVdVdtMatrix [expData.getLength()]; 
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		expVdVdtMatrices[nTrace] =  PabloVdVdtMatrix(expData[nTrace]);
	}

}

PabloFitnessCalculator::~PabloFitnessCalculator() {
	delete [] expVdVdtMatrices;
	exportFileStream.close();
}

double PabloFitnessCalculator::calculateFitness(const ModelTuningParameters & params) {

	double fitnessValue = 0;

	ModelResults results = model->runModel(params);

    for (int nTrace = 0; nTrace < results.getLength(); nTrace++) {
		PabloVdVdtMatrix modelVdVdtMatrix(results[nTrace]);
		fitnessValue += results[nTrace].getWeight() * expVdVdtMatrices[nTrace].compare(modelVdVdtMatrix);
    }

	numberOfEvaluations++;

	cout << "Calculated fitness of: " << params.toString() << ": " << fitnessValue << endl;
	
	if (exportFileStream.is_open()) {
		exportFileStream << numberOfEvaluations << " " << fitnessValue << " ";
		for (int i = 0; i < params.getLength(); i++) {	
			exportFileStream << params[i] << " ";
		}
		exportFileStream << endl;
	}
	
	return fitnessValue;
}

void PabloFitnessCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	cout << "Enable file export";
}

void PabloFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string PabloFitnessCalculator::getExportFile() const {
	return exportFile;
}

