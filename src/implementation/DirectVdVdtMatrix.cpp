/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../DirectVdVdtMatrix.h"

DirectVdVdtMatrix::DirectVdVdtMatrix() 
	: MapVdVdtMatrix() {}

DirectVdVdtMatrix::DirectVdVdtMatrix(FixedParameters params) 
	: MapVdVdtMatrix(params) {}

DirectVdVdtMatrix::DirectVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	MapVdVdtMatrix(trace, params) {}

double DirectVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const DirectVdVdtMatrix & other = dynamic_cast<const DirectVdVdtMatrix &>(o);

    double errorValue = 0;

    double diff = 0;

    const double precision = toDouble(fixedParams["comparePrecision"]);

    if (other.getVLength() != vLength) crash("DirectVdVdtMatrix","V dimensions don't match");
    if (other.getdVdtLength() != dVdtLength) crash("DirectVdVdtMatrix","dVdt dimensions don't match");


	if (toInt(fixedParams["SumOfSquareRoots"]) == 1) {
    	////////////////////////////////////////////////////////////
    	/// Calculate the square of the sum of the squares roots ///
    	////////////////////////////////////////////////////////////
		double worstValue = 0;
		double worstSpread;
	
		if (this->getMaxNumberOfPoints() > other.getMaxNumberOfPoints())
			worstSpread = ceil((double)this->getMaxNumberOfPoints()/(vLength*dVdtLength));
		else
			worstSpread = ceil((double)other.getMaxNumberOfPoints()/(vLength*dVdtLength));

    	for (int vIndex=0;vIndex<vLength;vIndex++) {
        	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            	diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
            	if (diff > precision) {
                	errorValue += pow(diff,0.5);
            	}
				worstValue += pow(worstSpread, 0.5);
        	}
    	}

		if (worstValue == 0) return errorValue = 1;
		errorValue = pow(errorValue/worstValue,2);
	}
	else {
    	///////////////////////////////////////////////////////////
    	/// Calculate the square root of the sum of the squares ///
    	///////////////////////////////////////////////////////////

		double worstValue = pow(pow((double)this->getMaxNumberOfPoints(),2)+pow((double)other.getMaxNumberOfPoints(),2),0.5);

    	for (int vIndex=0;vIndex<vLength;vIndex++) {
        	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            	diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
            	if (diff > precision) {
                	errorValue += pow(diff,2);
            	}
        	}
    	}

		if (worstValue == 0) return errorValue = 1;
		else errorValue = pow(errorValue,0.5)/worstValue;
    }

	                                                                                                    
    return errorValue;

}
