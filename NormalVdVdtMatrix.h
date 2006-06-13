#ifndef NEUROFITTER_NORMALVDVDTMATRIX_H
#define NEUROFITTER_NORMALVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "FixedParamObject.h"
#include "Tools.h"

///todo use a map to implement this
class NormalVdVdtMatrix : public FixedParamObject {

public:
	NormalVdVdtMatrix();
	NormalVdVdtMatrix(const FixedParameters);
	NormalVdVdtMatrix(const DataTrace&, const FixedParameters);

	int getVLength() const;
	int getdVdtLength() const;

	inline vector<double>& operator[](const int);
	inline const vector<double>& operator[](const int) const;

	double compare(const NormalVdVdtMatrix&) const;

	string toString() const;
	
protected:
	int vLength;
	int dVdtLength;	
	vector< vector< double > > VdVdtMatrix;
};
	
#endif
