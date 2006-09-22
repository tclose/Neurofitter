#ifndef NEUROFITTER_MAPVDVDTMATRIX_H
#define NEUROFITTER_MAPVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "VdVdtMatrix.h"
#include "FixedParamObject.h"
#include "Tools.h"

class MapVdVdtMatrix : public VdVdtMatrix {

public:
	MapVdVdtMatrix();
	MapVdVdtMatrix(const FixedParameters);
	MapVdVdtMatrix(const DataTrace&, const FixedParameters);

	const double get(const int v, const int dVdt) const;	
	void set(const int v, const int dVdt, double value);	

	void makeEmpty();

	double compare(const VdVdtMatrix&) const;
	
protected:
	map< int, double > matrix;

	int mIndex(const int v, const int dVdt) const;
	int vIndex(const int mIndex) const;
	int dVdtIndex(const int mIndex) const;	

};
	
#endif
