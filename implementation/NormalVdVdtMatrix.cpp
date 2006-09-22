#include <iostream>
#include <cmath>

#include "../Tools.h"

#include "../NormalVdVdtMatrix.h"

NormalVdVdtMatrix::NormalVdVdtMatrix() 
	: VdVdtMatrix() {}

NormalVdVdtMatrix::NormalVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params) {}

NormalVdVdtMatrix::NormalVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	VdVdtMatrix(params) {
		
	readFrom(trace);	

}

double NormalVdVdtMatrix::compare(const VdVdtMatrix & other) const {

	double fitnessValue = 0;

	double diff = 0;

	const double precision = toDouble(fixedParams["comparePrecision"]);

	if (other.getVLength() != vLength) crash("NormalVdVdtMatrix","V dimensions don't match");
	if (other.getdVdtLength() != dVdtLength) crash("NormalVdVdtMatrix","dVdt dimensions don't match");			

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

	return sqrt(fitnessValue);
}

inline const double NormalVdVdtMatrix::get(const int v, const int dVdt) const {
    if (v < 0 || v >= vLength) {
        crash("MapVdVdtMatrix","Invalid v: "+str(v));
    }
    if (dVdt < 0 || dVdt >= dVdtLength) {
        crash("MapVdVdtMatrix","Invalid dVdt: "+str(dVdt));
    }
    map< const int, double >::const_iterator i = matrix.find(mIndex(v,dVdt));
    if (i == matrix.end()) {
        return 0;                                                                                       
    }
    else {
        return i->second;
    }
}



void NormalVdVdtMatrix::set(const int v, const int dVdt, double value) {
    if (v < 0 || v >= vLength) {
        crash("NormalVdVdtMatrix","Invalid v: "+str(v));
    }
    if (dVdt < 0 || dVdt >= dVdtLength) {
        crash("NormalVdVdtMatrix","Invalid dVdt: "+str(dVdt));
    }
    matrix[mIndex(v,dVdt)] = value;
}

void NormalVdVdtMatrix::makeEmpty() {
    matrix.clear();
}

int NormalVdVdtMatrix::mIndex(const int v, const int dVdt) const {
    return v*dVdtLength + dVdt;
}

int NormalVdVdtMatrix::vIndex(const int mIndex) const {
    return mIndex / dVdtLength;
}

int NormalVdVdtMatrix::dVdtIndex(const int mIndex) const {
    return mIndex % dVdtLength;
}
