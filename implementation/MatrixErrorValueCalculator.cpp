/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../DirectVdVdtMatrix.h"
#include "../DistanceVdVdtMatrix.h"
#include "../ROIVdVdtMatrix.h"

#include "../MatrixErrorValueCalculator.h"

MatrixErrorValueCalculator::MatrixErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters params) 
	: ErrorValueCalculator(interface), FixedParamObject(params), modelVdVdtMatrix(NULL) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

	ModelResults expData = experiment->getData();	

	expVdVdtMatrices = vector< VdVdtMatrix * >(expData.getLength(), (VdVdtMatrix*)NULL); 
	
	showMessage("Experiment VdVdtMatrices\n",5,fixedParams);
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		if (fixedParams["VdVdtMatrixType"] == "Direct") {
			expVdVdtMatrices[nTrace] = new DirectVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
		}
		else if (fixedParams["VdVdtMatrixType"] == "Distance") {
			expVdVdtMatrices[nTrace] = new DistanceVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
		}
		else if (fixedParams["VdVdtMatrixType"] == "ROI") {
			expVdVdtMatrices[nTrace] = new ROIVdVdtMatrix(expData[nTrace], FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
		}
		else { 
			crash("MatrixErrorValueCalculator", "No matching VdVdtmatrix type: " + fixedParams["VdVdtMatrixType"]);
		}
		showMessage(expVdVdtMatrices[nTrace]->toString()+"\n",5,fixedParams);
	}

    if (fixedParams["VdVdtMatrixType"] == "Direct") {
		modelVdVdtMatrix = new DirectVdVdtMatrix(FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
	}
    else if (fixedParams["VdVdtMatrixType"] == "Distance") {
		modelVdVdtMatrix = new DistanceVdVdtMatrix(FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
	}
    else if (fixedParams["VdVdtMatrixType"] == "ROI") {
		modelVdVdtMatrix = new ROIVdVdtMatrix(FixedParameters(fixedParams["VdVdtMatrixParameters"],fixedParams.getGlobals()));
	}
	else {
		crash("MatrixErrorValueCalculator", "No matching VdVdtmatrix type: " + fixedParams["VdVdtMatrixType"]);
    }


}

MatrixErrorValueCalculator::~MatrixErrorValueCalculator() {
	if (modelVdVdtMatrix != NULL) delete modelVdVdtMatrix;
	for (int i = 0; i < (int)expVdVdtMatrices.size(); i++) {
		if (expVdVdtMatrices[i] != NULL) delete expVdVdtMatrices[i];
	}
	exportFileStream.close();
}

void MatrixErrorValueCalculator::calculateErrorValue(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	calculateParallelErrorValue(paramList);
	
	/// This is necessary because otherwise the error value is not transferred 
	/// since no reference is passed to calculateParallelErrorValue
	params.setErrorValue(paramList[0].getErrorValue());

}

void MatrixErrorValueCalculator::calculateParallelErrorValue(vector< ModelTuningParameters > & paramList) {

	vector< double > errorValues(paramList.size());

    vector< ModelResults > results = model->runParallelModel(paramList);

	for (unsigned int i = 0; i < paramList.size(); i++) {
		showMessage("Model VdVdtMatrices\n",5,fixedParams);

    	for (int nTrace = 0; nTrace < results[i].getLength(); nTrace++) {
        	modelVdVdtMatrix->readFrom(results[i][nTrace]);
        	errorValues[i] += results[i][nTrace].getWeight() * expVdVdtMatrices[nTrace]->compare(*modelVdVdtMatrix);
			showMessage(modelVdVdtMatrix->toString() + "\n",5,fixedParams);        	
    	}

    	numberOfEvaluations++;

    	errorValueHistory.push_back(pair< ModelTuningParameters, double >(paramList[i],errorValues[i]));

		showMessage("Eval: " + str(numberOfEvaluations) + " Generation: " + str(numberOfGenerations) + " Calculated error value of: " + paramList[i].toString() + ": " + str(errorValues[i]) + "\n",3,fixedParams);

    	if (exportFileStream.is_open()) {
        	exportFileStream << numberOfGenerations << " "<< numberOfEvaluations << " " << errorValues[i] << " ";
        	for (int j = 0; j < paramList[i].getLength(); j++) {
            	exportFileStream << (paramList[i][j]) << " ";
        	}
        	exportFileStream << endl;
    	}

    	paramList[i].setErrorValue(errorValues[i]);

	}
	
	numberOfGenerations++;

}

vector< pair< ModelTuningParameters, double > > MatrixErrorValueCalculator::getErrorValueHistory() {
	return errorValueHistory;
}


void MatrixErrorValueCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	
	showMessage("MatrixErrorValueCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);        	
}

void MatrixErrorValueCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MatrixErrorValueCalculator::getExportFile() const {
	return exportFile;
}
