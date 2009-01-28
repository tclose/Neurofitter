/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../ROIStringParser.h"

#include "../ROI.h"
#include "../ROIVdVdtMatrix.h"

ROIVdVdtMatrix::ROIVdVdtMatrix() 
	: VdVdtMatrix() {}

ROIVdVdtMatrix::ROIVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params) {}

ROIVdVdtMatrix::ROIVdVdtMatrix(DataTrace & trace, FixedParameters params) 
	: VdVdtMatrix(params) {

	readFrom(trace);

}

void ROIVdVdtMatrix::makeEmpty() {

	matrices.clear();

}

double ROIVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const ROIVdVdtMatrix & other = dynamic_cast<const ROIVdVdtMatrix &>(o);

	double errorValue = 0;

	if (other.matrices.size() != matrices.size()) crash("ROIVdVdtMatrix","Matrices length sizes don't match");

	for (unsigned int i = 0; i < matrices.size(); i++) {
		errorValue += matrices[i].getWeight() * matrices[i].compare(other.matrices[i]);
		//showMessage("Comparision: "+str(matrices[i].compare(other.matrices[i]))+" Weight: "+str(matrices[i].getWeight())+"\n");
	}
	
	return errorValue;

}

void ROIVdVdtMatrix::readFrom(const DataTrace& trace) {
		makeEmpty();

		ROIsParser.initialize(fixedParams["ROILines"], toInt(fixedParams["NumberOfROILines"]), trace.getNumberOfRuns(), trace.getNumberOfPeriods(), trace.getNumberOfRecordingSites());
	
		showMessage("ROIs parsed: " + ROIsParser.toString(),14,fixedParams);	
		
		vector< ROI > ROIs = ROIsParser.getROIsForRunAndPeriodAndRecord(trace.getRun(), trace.getPeriod(), trace.getRecordingSite());
		for (vector< ROI >::const_iterator iter = ROIs.begin(); iter != ROIs.end(); iter++) {
			DirectVdVdtMatrix matrix(fixedParams);
			matrix.setToROI(*iter);
			matrix.setWeight(iter->getWeight());
			matrix.readFrom(trace);
			matrices.push_back(matrix);
		}
		
	}

string ROIVdVdtMatrix::toString() const {

	string returnString = "";
	int nROI = 0;
	for (vector< DirectVdVdtMatrix >::const_iterator iter = matrices.begin(); iter != matrices.end(); iter++) {
		returnString += "ROI " + str(nROI) + ": \n" + iter->toString() + "\n";
		nROI++;
	}
	return returnString;

}

string ROIVdVdtMatrix::toFileExportString() const {

	string returnString = "";
	int nROI = 0;
	for (vector< DirectVdVdtMatrix >::const_iterator iter = matrices.begin(); iter != matrices.end(); iter++) {
		returnString += iter->toFileExportString() + " ";
		nROI++;
	}
	returnString = str(nROI) + " " + returnString;
	return returnString;

}
