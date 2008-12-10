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
	if (toInt(fixedParams["enableTracesExport"]) > 0) {
		this->enableTracesExport(fixedParams["tracesFile"]);
	}

}

RodrigoErrorValueCalculator::~RodrigoErrorValueCalculator() {
	exportFileStream.close();
	tracesFileStream.close();
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

		if (results[i].getLength() != 5) {
			crash("RodrigoErrorValueCalculator","Didn't get exactly three traces: "+str(results[i].getLength()));
		}
		
		DataTrace & Cai = results[i][0]; 
		DataTrace & LTDModel = results[i][1]; 
		DataTrace & LTDExp = results[i][2];
		DataTrace & Inputcurrent = results[i][3];
		DataTrace & Frequency = results[i][4];
       
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

    	if (tracesFileStream.is_open()) {
        	tracesFileStream << numberOfGenerations << " "<< numberOfEvaluations << " " << errorValues[i] << " ";
        	for (unsigned j = 0; j < paramList[i].getLength(); j++) {
            	tracesFileStream << (paramList[i][j]) << " ";
        	}
        	for (int j = 0; j < Cai.getLength(); j++) {
            	tracesFileStream << (Cai.get(j)) << " " << (LTDModel.get(j)) << " " << (LTDExp.get(j)) << " " << (Inputcurrent.get(j)) << " " << (Frequency.get(j)) << " ";
        	}
        	tracesFileStream << endl;
    	}

    	paramList[i].setErrorValue(errorValues[i]);
    	paramList[i].setMOErrorValues(MOErrorValues[i]);

	}
	
	numberOfGenerations++;

}

double RodrigoErrorValueCalculator::calculateRMSError(DataTrace & LTDModel, DataTrace & LTDExp) {

	double rmsError = 0;

	double maxValue = 0;

	for (int i = 0; i < LTDModel.getLength(); i++) {
		if (LTDModel.get(i) > maxValue) maxValue = LTDModel.get(i);
	}

	if (maxValue == 0) maxValue = 1;

	for (int i = 0; i < LTDModel.getLength(); i++) {
		rmsError += (LTDModel.get(i)/maxValue - LTDExp.get(i))*(LTDModel.get(i)/maxValue - LTDExp.get(i));
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

void RodrigoErrorValueCalculator::enableTracesExport(const string fileName) {
	
	//When in MPI mode append the rank to the filename;
    #ifdef WITH_MPI
        string fileLoc = fileName + "." + fixedParams["Rank"];
    #else
        string fileLoc = fileName;
    #endif


	tracesFileStream.open(fileLoc.c_str(), ios::out);
	if (!tracesFileStream.good()) crash("RodrigoErrorValueCalculator","Can't open traces file");
	
	showMessage("RodrigoErrorValueCalculator: Enabled traces export to file: " + fileName + "\n",3,fixedParams);        	
}

void RodrigoErrorValueCalculator::disableTracesExport() {
	tracesFileStream.close();
}
   
