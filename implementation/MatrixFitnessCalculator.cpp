#include "../MatrixFitnessCalculator.h"

MatrixFitnessCalculator::MatrixFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters params) 
	: FitnessCalculator(interface), FixedParamObject(params) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

	ModelResults expData = experiment->getData();	

	expVdVdtMatrices = vector< PabloVdVdtMatrix >(expData.getLength()); 
	if (toInt(fixedParams["VerboseLevel"]) > 4) {
		cout << "Experiment VdVdtMatrices: "<< endl;
	}
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		expVdVdtMatrices[nTrace] = PabloVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["PabloVdVdtMatrix"],fixedParams.getGlobals()));
		if (toInt(fixedParams["VerboseLevel"]) > 4) {
			cout << expVdVdtMatrices[nTrace].toString() << endl;
        }
	}

}

MatrixFitnessCalculator::~MatrixFitnessCalculator() {
	exportFileStream.close();
}

/// todo don't return value, but use value inside ModelTuningParameters
double MatrixFitnessCalculator::calculateFitness(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	return (calculateParallelFitness(paramList))[0];

}

vector< double > MatrixFitnessCalculator::calculateParallelFitness(vector< ModelTuningParameters > & paramList) {

	vector< double > fitnessValues(paramList.size());

    vector< ModelResults > results = model->runParallelModel(paramList);

	for (unsigned int i = 0; i < paramList.size(); i++) {
    	if (toInt(fixedParams["VerboseLevel"]) > 4) {
        	cout << "Model VdVdtMatrices: " << endl;
    	}
    	for (int nTrace = 0; nTrace < results[i].getLength(); nTrace++) {
        	PabloVdVdtMatrix modelVdVdtMatrix(results[i][nTrace], FixedParameters(fixedParams["PabloVdVdtMatrix"], fixedParams.getGlobals()));
        	fitnessValues[i] += results[i][nTrace].getWeight() * expVdVdtMatrices[nTrace].compare(modelVdVdtMatrix);
        	if (toInt(fixedParams["VerboseLevel"]) > 4) {
            	cout << modelVdVdtMatrix.toString() << endl;
        	}
    	}
    	numberOfEvaluations++;

    	fitnessHistory.push_back(pair< ModelTuningParameters, double >(paramList[i],fitnessValues[i]));

    	if (toInt(fixedParams["VerboseLevel"]) > 2) {
        	cout << "Calculated fitness of: " << paramList[i].toString() << ": " << fitnessValues[i] << endl;
    	}

    	if (exportFileStream.is_open()) {
        	exportFileStream << numberOfGenerations << " "<< numberOfEvaluations << " " << fitnessValues[i] << " ";
        	for (int j = 0; j < paramList[i].getLength(); j++) {
            	exportFileStream << (paramList[i][j]) << " ";
        	}
        	exportFileStream << endl;
    	}

    	paramList[i].setFitnessValue(fitnessValues[i]);

	}
	
	numberOfGenerations++;

    return fitnessValues;

}

vector< pair< ModelTuningParameters, double > > MatrixFitnessCalculator::getFitnessHistory() {
	return fitnessHistory;
}


void MatrixFitnessCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	if (toInt(fixedParams["VerboseLevel"]) > 2) {
		cout << "MatrixFitnessCalculator: Enabled export to file: " << fileName << endl;
	}
}

void MatrixFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MatrixFitnessCalculator::getExportFile() const {
	return exportFile;
}
