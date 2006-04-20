#include "../PabloFitnessCalculator.h"

static int numberOfEvaluations = 0;

PabloFitnessCalculator::PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment, const FixedParameters params) 
	: FixedParamObject(params) {
	model = interface;

	FixedParameters globalParams;
    globalParams["VerboseLevel"] = fixedParams["VerboseLevel"];

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

	ModelResults expData = experiment->getData();
	
	expVdVdtMatrices = new PabloVdVdtMatrix [expData.getLength()]; 
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		expVdVdtMatrices[nTrace] = PabloVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["PabloVdVdtMatrix"],globalParams));
		if (toInt(fixedParams["VerboseLevel"]) > 4) {
            cout << "Experiment VdVdtMatrices: "<< endl << expVdVdtMatrices[nTrace].toString() << endl;
        }
	}

}

PabloFitnessCalculator::~PabloFitnessCalculator() {
	delete [] expVdVdtMatrices;
	exportFileStream.close();
}

double PabloFitnessCalculator::calculateFitness(const ModelTuningParameters & params) {

	double fitnessValue = 0;
	FixedParameters globalParams;
	globalParams["VerboseLevel"] = fixedParams["VerboseLevel"]; 

	ModelResults results = model->runModel(params);

    for (int nTrace = 0; nTrace < results.getLength(); nTrace++) {
		PabloVdVdtMatrix modelVdVdtMatrix(results[nTrace], FixedParameters(fixedParams["PabloVdVdtMatrix"], globalParams));
		fitnessValue += results[nTrace].getWeight() * expVdVdtMatrices[nTrace].compare(modelVdVdtMatrix);
		if (toInt(fixedParams["VerboseLevel"]) > 4) {
			cout << "Experiment VdVdtMatrices: " << endl << modelVdVdtMatrix.toString() << endl;
		}
    }

	numberOfEvaluations++;

	if (toInt(fixedParams["VerboseLevel"]) > 2) {
		cout << "Calculated fitness of: " << params.toString() << ": " << fitnessValue << endl;
	}

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
	if (toInt(fixedParams["VerboseLevel"]) > 2) {
		cout << "PabloFitnessCalculator: Enabled export to file: " << fileName << endl;
	}
}

void PabloFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string PabloFitnessCalculator::getExportFile() const {
	return exportFile;
}
