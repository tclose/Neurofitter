/*
Revision of last commit: $Rev: 285 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2008-08-26 09:47:24 +0900 (Tue, 26 Aug 2008) $
*/

#include "../RMSErrorValueCalculator.h"

#include <cmath>

RMSErrorValueCalculator::RMSErrorValueCalculator(ModelInterface * interface, ExperimentInterface * experiment, FixedParameters params) 
	: ErrorValueCalculator(interface), FixedParamObject(params), expData(experiment->getData()) {

	if (toInt(fixedParams["enableFileExport"]) > 0) {
		this->enableFileExport(fixedParams["exportFile"]);
	}

}

RMSErrorValueCalculator::~RMSErrorValueCalculator() {
	exportFileStream.close();
	tracesFileStream.close();
}

void RMSErrorValueCalculator::calculateErrorValue(ModelTuningParameters & params) {

	vector< ModelTuningParameters > paramList(1);
	paramList[0] = params;
	
	calculateParallelErrorValue(paramList);
	
	/// This is necessary because otherwise the error value is not transferred 
	/// since no reference is passed to calculateParallelErrorValue
	params = paramList[0];

}

void RMSErrorValueCalculator::calculateParallelErrorValue(vector< ModelTuningParameters > & paramList) {

	vector< double > errorValues(paramList.size());
	vector< vector< double > > MOErrorValues(paramList.size());

  vector< ModelResults > results(paramList.size());

	model->runParallelModel(paramList, results);

	for (unsigned int i = 0; i < paramList.size(); i++) {
		double rmsError;

		MOErrorValues[i] = vector< double >(results[i].getLength());

    for (int nTrace = 0; nTrace < results[i].getLength(); nTrace++) {
  		rmsError = calculateRMSError(results[i][nTrace], expData[nTrace]); 
	  	errorValues[i] += rmsError;
		  MOErrorValues[i][0] = rmsError;
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

    	paramList[i].setErrorValue(errorValues[i]);
    	paramList[i].setMOErrorValues(MOErrorValues[i]);

	}
	
	numberOfGenerations++;

}

double RMSErrorValueCalculator::calculateRMSError(DataTrace & trace1, DataTrace & trace2) {


	if (trace1.getLength() != trace2.getLength()) crash("RMSErrorValueCalculator","Calculating RMS Error between traces of different length");

	double rmsError = 0;

	for (int i = 0; i < trace1.getLength(); i++) {
		rmsError += (trace1.get(i) - trace2.get(i))*(trace1.get(i) - trace2.get(i));
	}

	rmsError /= trace1.getLength();
	rmsError = pow(rmsError,0.5);

	return rmsError;
}

vector< pair< ModelTuningParameters, double > > RMSErrorValueCalculator::getErrorValueHistory() {
	return errorValueHistory;
}


void RMSErrorValueCalculator::enableFileExport(const string fileName) {
	exportFileStream.open(fileName.c_str(), ios::out);
	if (!exportFileStream.good()) crash("RMSErrorValueCalculator","Can't open export file");
	
	showMessage("RMSErrorValueCalculator: Enabled export to file: " + fileName + "\n",3,fixedParams);        	
}

void RMSErrorValueCalculator::disableFileExport() {
	exportFileStream.close();
}

string RMSErrorValueCalculator::getExportFile() const {
	return exportFile;
}

void RMSErrorValueCalculator::enableTracesExport(const string fileName) {
	
	//When in MPI mode append the rank to the filename;
    #ifdef WITH_MPI
        string fileLoc = fileName + "." + fixedParams["Rank"];
    #else
        string fileLoc = fileName;
    #endif


	tracesFileStream.open(fileLoc.c_str(), ios::out);
	if (!tracesFileStream.good()) crash("RMSErrorValueCalculator","Can't open traces file");
	
	showMessage("RMSErrorValueCalculator: Enabled traces export to file: " + fileName + "\n",3,fixedParams);        	
}

void RMSErrorValueCalculator::disableTracesExport() {
	tracesFileStream.close();
}
