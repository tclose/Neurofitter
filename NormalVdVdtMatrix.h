#ifndef NEUROFITTER_NORMALVDVDTMATRIX_H
#define NEUROFITTER_NORMALVDVDTMATRIX_H

using namespace std;

#include "DataTrace.h"
#include "VdVdtMatrix.h"

class NormalVdVdtMatrix : public VdVdtMatrix  {

public:
	NormalVdVdtMatrix();
	NormalVdVdtMatrix(const FixedParameters);
	NormalVdVdtMatrix(const DataTrace&, const FixedParameters);

	int getVLength() const;
	int getdVdtLength() const;

	virtual void set(const int v, const int dVdt, const double value);
	virtual const double get(const int v, const int dVdt) const;

	virtual double compare(const VdVdtMatrix&) const;
	
protected:
	vector< vector< double > > matrix;

};
	
#endif
