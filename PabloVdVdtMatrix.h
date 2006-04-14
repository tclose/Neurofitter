#ifndef PABLOVDVDTMATRIX_H
#define PABLOVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "Tools.h"

class PabloVdVdtMatrix {

public:
	PabloVdVdtMatrix();
	PabloVdVdtMatrix(const DataTrace&);
	PabloVdVdtMatrix(const PabloVdVdtMatrix & v) {*this = v;}; 
	
	PabloVdVdtMatrix& operator=(const PabloVdVdtMatrix&);

	~PabloVdVdtMatrix();

	int getVLength() const;
	int getdVdtLength() const;

	inline double * operator[](const int);
	inline double const * const  operator[](const int) const;

	double compare(const PabloVdVdtMatrix&) const;
	
private:
	double ** VdVdtMatrix;
	int vLength;
	int dVdtLength;	
};
	
#endif
