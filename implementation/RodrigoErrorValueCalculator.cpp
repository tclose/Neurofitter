/*
Revision of last commit: $Rev: 285 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2008-08-26 09:47:24 +0900 (Tue, 26 Aug 2008) $
*/

#include "../RodrigoErrorValueCalculator.h"

#include <cmath>

RodrigoErrorValueCalculator::RodrigoErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters params) 
	: ErrorValueCalculator(interface), FixedParamObject(params) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

}

RodrigoErrorValueCalculator::~RodrigoErrorValueCalculator() {
	exportFileStream.close();
}

void RodrigoErrorValueCalculator::calculateErrorValue(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	calculateParallelErrorValue(paramList);
	
	/// This is necessary because otherwise the error value is not transferred 
	/// since no reference is passed to calculateParallelErrorValue
	params = paramList[0];

}

void RodrigoErrorValueCalculator::calculateParallelErrorValue(vector< ModelTuningParameters > & paramList) {

	vector< double > errorValues(paramList.size());
	vector< vector< double > > MOErrorValues(paramList.size());

    vector< ModelResults > results(paramList.size());

	model->runParallelModel(paramList, results);

	for (unsigned int i = 0; i < paramList.size(); i++) {
		//showMessage("Model VdVdtMatrices\n",5,fixedParams);

		double rmsError;

		MOErrorValues[i] = vector< double >(1);

		if (results[i].getLength() != 3) {
			crash("RodrigoErrorValueCalculator","Didn't get exactly three traces: "+str(results[i].getLength()));
		}
		
		//DataTrace & Cai = results[i][0]; 
		DataTrace & LTDModel = results[i][1]; 
		DataTrace & LTDExp = results[i][2];
       
		rmsError = calculateRMSError(LTDModel, LTDExp); 
		errorValues[i] = rmsError;
		MOErrorValues[i][0] = rmsError;

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

    	paramList[i].setErrorValue(errorValues[i]);
    	paramList[i].setMOErrorValues(MOErrorValues[i]);

	}
	
	numberOfGenerations++;

}

double RodrigoErrorValueCalculator::calculateRMSError(DataTrace & LTDModel, DataTrace & LTDExp) {

	double rmsError = 0;

	for (int i = 0; i < LTDModel.getLength(); i++) {
		rmsError += (LTDModel.get(i)-LTDExp.get(i))*(LTDModel.get(i)-LTDExp.get(i));
	}
	rmsError /= LTDModel.getLength();
	rmsError = pow(rmsError,0.5);

	return rmsError;
}

vector< pair< ModelTuningParameters, double > > RodrigoErrorValueCalculator::getErrorValueHistory() {
	return errorValueHistory;
}


void RodrigoErrorValueCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	if (!exportFileStream.good()) crash("RodrigoErrorValueCalculator","Can't open export file");
	
	showMessage("RodrigoErrorValueCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);        	
}

void RodrigoErrorValueCalculator::disableFileExport() {
	exportFileStream.close();
}
   
string RodrigoErrorValueCalculator::getExportFile() const {
	return exportFile;
}
