#include "../MapVdVdtMatrix.h"

MapVdVdtMatrix::MapVdVdtMatrix() 
	: VdVdtMatrix() {}

MapVdVdtMatrix::MapVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params) {}

MapVdVdtMatrix::MapVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	VdVdtMatrix(params) { 

	readFrom(trace);		

}

MapVdVdtMatrix::~MapVdVdtMatrix() {}

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
