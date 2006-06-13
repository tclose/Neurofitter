#ifndef PABLOVDVDTMATRIX_H
#define PABLOVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "FixedParamObject.h"
#include "Tools.h"

///todo use a map to implement this
class VdVdtMatrix : public FixedParamObject {

public:
	VdVdtMatrix();
	VdVdtMatrix(const FixedParameters);
	VdVdtMatrix(const DataTrace&, const FixedParameters);

	int getVLength() const;
	int getdVdtLength() const;

	inline vector<double>& operator[](const int);
	inline const vector<double>& operator[](const int) const;

	double compare(const VdVdtMatrix&) const;

	string toString() const;
	
protected:
	int vLength;
	int dVdtLength;	
	vector< vector< double > > VdVdtMatrix;
};
	
#endif
