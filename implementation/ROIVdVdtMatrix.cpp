/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../ROIStringParser.h"

#include "../ROIVdVdtMatrix.h"

ROIVdVdtMatrix::ROIVdVdtMatrix() 
	: VdVdtMatrix() {}

ROIVdVdtMatrix::ROIVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params) {}

ROIVdVdtMatrix::ROIVdVdtMatrix(DataTrace & trace, FixedParameters params) 
	: VdVdtMatrix(params), ROIs(params["ROILines"], toInt(params["NumberOfROILines"]), trace.getNumberOfRuns(), trace.getNumberOfPeriods()) {readFrom(trace);}

void ROIVdVdtMatrix::makeEmpty() {

	matrices.clear();

}

double ROIVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const ROIVdVdtMatrix & other = dynamic_cast<const ROIVdVdtMatrix &>(o);

	double errorValue = 0;

	if (other.matrices.size() != matrices.size()) crash("DirectVdVdtMatrix","Matrices length sizes don't match");

	for (unsigned int i = 0; i < matrices.size(); i++) {
		errorValue += matrices[i].getWeight() * matrices[i].compare(other.matrices[i]);
	}
	
	return errorValue;

}
