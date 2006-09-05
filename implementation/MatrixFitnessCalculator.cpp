#include "../MatrixFitnessCalculator.h"

MatrixFitnessCalculator::MatrixFitnessCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters params) 
	: FitnessCalculator(interface), FixedParamObject(params) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

	ModelResults expData = experiment->getData();	

	expVdVdtMatrices = vector< NormalVdVdtMatrix >(expData.getLength()); 
	
	showMessage("Experiment VdVdtMatrices\n",5,fixedParams);
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		expVdVdtMatrices[nTrace] = NormalVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["NormalVdVdtMatrix"],fixedParams.getGlobals()));
		showMessage(expVdVdtMatrices[nTrace].toString()+"\n",5,fixedParams);
	}

}

MatrixFitnessCalculator::~MatrixFitnessCalculator() {
	exportFileStream.close();
}

void MatrixFitnessCalculator::calculateFitness(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	calculateParallelFitness(paramList);
	
	/// This is necessary because otherwise the fitnessvalue is not transferred 
	/// since no reference is passed to calculateParallelFitness
	params.setFitnessValue(paramList[0].getFitnessValue());

}

void MatrixFitnessCalculator::calculateParallelFitness(vector< ModelTuningParameters > & paramList) {

	vector< double > fitnessValues(paramList.size());

    vector< ModelResults > results = model->runParallelModel(paramList);

	for (unsigned int i = 0; i < paramList.size(); i++) {
		showMessage("Model VdVdtMatrices\n",5,fixedParams);

    	for (int nTrace = 0; nTrace < results[i].getLength(); nTrace++) {
        	NormalVdVdtMatrix modelNormalVdVdtMatrix(results[i][nTrace], FixedParameters(fixedParams["NormalVdVdtMatrix"], fixedParams.getGlobals()));
        	fitnessValues[i] += results[i][nTrace].getWeight() * expVdVdtMatrices[nTrace].compare(modelNormalVdVdtMatrix);
			showMessage(modelNormalVdVdtMatrix.toString() + "\n",5,fixedParams);        	
    	}

    	numberOfEvaluations++;

    	fitnessHistory.push_back(pair< ModelTuningParameters, double >(paramList[i],fitnessValues[i]));

		showMessage("Calculated fitness of: " + paramList[i].toString() + ": " + str(fitnessValues[i]) + "\n",3,fixedParams);        	

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

}

vector< pair< ModelTuningParameters, double > > MatrixFitnessCalculator::getFitnessHistory() {
	return fitnessHistory;
}


void MatrixFitnessCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	
	showMessage("MatrixFitnessCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);        	
}

void MatrixFitnessCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MatrixFitnessCalculator::getExportFile() const {
	return exportFile;
}
