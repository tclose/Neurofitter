#include "../MapVdVdtMatrix.h"

MapVdVdtMatrix::MapVdVdtMatrix() 
	: VdVdtMatrix() {}

MapVdVdtMatrix::MapVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params) {}

MapVdVdtMatrix::MapVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	VdVdtMatrix(params) { 

	readFrom(trace);		

}

double MapVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const MapVdVdtMatrix & other = dynamic_cast<const MapVdVdtMatrix &>(o);

    double fitnessValue = 0;

    double diff = 0;

    const double precision = toDouble(fixedParams["comparePrecision"]);

    if (other.getVLength() != vLength) crash("MapVdVdtMatrix","V dimensions don't match");
    if (other.getdVdtLength() != dVdtLength) crash("MapVdVdtMatrix","dVdt dimensions don't match");

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


inline const double MapVdVdtMatrix::get(const int v, const int dVdt) const {
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

inline void MapVdVdtMatrix::set(const int v, const int dVdt, double value) {
    if (v < 0 || v >= vLength) {
        crash("MapVdVdtMatrix","Invalid v: "+str(v));
    }
    if (dVdt < 0 || dVdt >= dVdtLength) {
        crash("MapVdVdtMatrix","Invalid dVdt: "+str(dVdt));
    }
	matrix[mIndex(v,dVdt)] = value;
}

void MapVdVdtMatrix::makeEmpty() {
	matrix.clear();
}

int MapVdVdtMatrix::mIndex(const int v, const int dVdt) const {
	return v*dVdtLength + dVdt;
}

int MapVdVdtMatrix::vIndex(const int mIndex) const {
	return mIndex / dVdtLength;
}
    
int MapVdVdtMatrix::dVdtIndex(const int mIndex) const {
	return mIndex % dVdtLength;
}
