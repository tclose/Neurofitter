#include "../DirectVdVdtMatrix.h"

DirectVdVdtMatrix::DirectVdVdtMatrix() 
	: MapVdVdtMatrix() {}

DirectVdVdtMatrix::DirectVdVdtMatrix(FixedParameters params) 
	: MapVdVdtMatrix(params) {}

DirectVdVdtMatrix::DirectVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	MapVdVdtMatrix(trace, params) {}

double DirectVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const DirectVdVdtMatrix & other = dynamic_cast<const DirectVdVdtMatrix &>(o);

    double fitnessValue = 0;

    double diff = 0;

    const double precision = toDouble(fixedParams["comparePrecision"]);

    if (other.getVLength() != vLength) crash("DirectVdVdtMatrix","V dimensions don't match");
    if (other.getdVdtLength() != dVdtLength) crash("DirectVdVdtMatrix","dVdt dimensions don't match");

    ///////////////////////////////////////////////////////////
    /// Calculate the square root of the sum of the squares ///
    ///////////////////////////////////////////////////////////

    for (int vIndex=0;vIndex<vLength;vIndex++) {
        for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
            if (diff > precision) {
                fitnessValue += pow(diff,2);
            }
        }
    }
                                                                                                        
    return pow(fitnessValue,0.5);

}
