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
	: ErrorValueCalculator(interface), FixedParamObject(params), modelVdVdtMatrix(NULL), printMatrix(false), filePrintMatrix(false) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

	if (fixedParams.parameterExists("enableMatrixFileExport")) {
		if (toInt(fixedParams["enableMatrixFileExport"]) > 0) {
			this->enableMatrixFileExport(fixedParams["matrixExportFile"]);
		}
	}

	if (fixedParams.parameterExists("enablePrintMatrix")) {
		if (toInt(fixedParams["enablePrintMatrix"]) > 0) {
			printMatrix = true;	
		}
		else {
			printMatrix = false;
		}
	}
	else if (toInt(fixedParams["VerboseLevel"]) >= 5) {
		printMatrix = true;
	}
	else {
		printMatrix = false;
	}

	ModelResults expData = experiment->getData();	

	expVdVdtMatrices = vector< VdVdtMatrix * >(expData.getLength(), (VdVdtMatrix*)NULL); 
	
	showMessage("Experiment VdVdtMatrices\n",5,fixedParams);

	if (filePrintMatrix) initMatrixLine(ModelTuningParameters(toInt(fixedParams["Dimensions"])), 1, expData[0]);

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
		if (printMatrix) showMessage(expVdVdtMatrices[nTrace]->toString()+"\n");
		if (filePrintMatrix) saveMatrix(expData[nTrace], expVdVdtMatrices[nTrace]);
	}
	
	if (matrixExportFileStream.is_open()) matrixExportFileStream << " 0" << endl;

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
	params = paramList[0];

}

void MatrixErrorValueCalculator::calculateParallelErrorValue(vector< ModelTuningParameters > & paramList) {

	vector< double > errorValues(paramList.size());
	vector< vector< double > > MOErrorValues(paramList.size());

    vector< ModelResults > results(paramList.size());

	model->runParallelModel(paramList, results);

	for (unsigned int i = 0; i < paramList.size(); i++) {
		showMessage("Model VdVdtMatrices\n",5,fixedParams);

		MOErrorValues[i] = vector< double >(results[i].getLength());

		if (filePrintMatrix) initMatrixLine(paramList[i], 0, results[i][0]);

    	for (int nTrace = 0; nTrace < results[i].getLength(); nTrace++) {
        	modelVdVdtMatrix->readFrom(results[i][nTrace]);
			MOErrorValues[i][nTrace] = results[i][nTrace].getWeight() * expVdVdtMatrices[nTrace]->compare(*modelVdVdtMatrix);
        	errorValues[i] += MOErrorValues[i][nTrace];
			if (printMatrix) showMessage(modelVdVdtMatrix->toString() + "\n");
			if (filePrintMatrix) saveMatrix(results[i][nTrace], modelVdVdtMatrix);
    	}

    	numberOfEvaluations++;

    	errorValueHistory.push_back(pair< ModelTuningParameters, double >(paramList[i],errorValues[i]));

		showMessage("Eval: " + str(numberOfEvaluations) + " Generation: " + str(numberOfGenerations) + " Calculated error value of: " + paramList[i].toString() + ": " + str(errorValues[i]) + "\n",3,fixedParams);

    	if (exportFileStream.is_open()) {
        	exportFileStream << numberOfGenerations << " "<< numberOfEvaluations << " " << errorValues[i] << " ";
        	for (unsigned j = 0; j < paramList[i].getLength(); j++) {
            	exportFileStream << (paramList[i][j]) << " ";
        	}
        	exportFileStream << endl;
    	}

		if (matrixExportFileStream.is_open()) matrixExportFileStream << " " << errorValues[i] << endl;

    	paramList[i].setErrorValue(errorValues[i]);
    	paramList[i].setMOErrorValues(MOErrorValues[i]);

	}
	
	numberOfGenerations++;

}

void MatrixErrorValueCalculator::initMatrixLine(ModelTuningParameters params, bool isExperimental, DataTrace trace) {
	if (isExperimental) {
		matrixExportFileStream << "1 ";
		matrixExportFileStream << params.getLength() << " ";
		for (unsigned i = 0; i < params.getLength(); i++) {
			matrixExportFileStream << "0 ";
		}
	}
	else {
		matrixExportFileStream << "0 ";
		matrixExportFileStream << params.getLength() << " ";
		for (unsigned i = 0; i < params.getLength(); i++) {
			matrixExportFileStream << params[i] << " ";
		}
	}
	matrixExportFileStream << trace.getNumberOfRuns() << " " << trace.getNumberOfPeriods() << " " << trace.getNumberOfRecordingSites() << " ";
	
}

void MatrixErrorValueCalculator::saveMatrix(DataTrace trace, VdVdtMatrix * matrix) {

	matrixExportFileStream << trace.getRun() << " " << trace.getPeriod() << " " << trace.getRecordingSite() << " ";
	matrixExportFileStream << matrix->toFileExportString() << " ";

}

vector< pair< ModelTuningParameters, double > > MatrixErrorValueCalculator::getErrorValueHistory() {
	return errorValueHistory;
}


void MatrixErrorValueCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	if (!exportFileStream.good()) crash("MatrixErrorValueCalculator","Can't open export file");
	
	showMessage("MatrixErrorValueCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);        	
}

void MatrixErrorValueCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string MatrixErrorValueCalculator::getExportFile() const {
	return exportFile;
}

void MatrixErrorValueCalculator::enableMatrixFileExport(const string fileName) {
	matrixExportFileStream.open(fileName.c_str(), ios::out);
	if (!matrixExportFileStream.good()) crash("MatrixErrorValueCalculator","Can't open matrix export file");

	filePrintMatrix = true;
	
	showMessage("MatrixErrorValueCalculator: Enabled matrix export to file: " + fileName + "\n",3,fixedParams);        	
}

void MatrixErrorValueCalculator::disableMatrixFileExport() {
	matrixExportFileStream.close();
	filePrintMatrix = false;
}
