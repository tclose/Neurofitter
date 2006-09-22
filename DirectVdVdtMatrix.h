#ifndef NEUROFITTER_DIRECTVDVDTMATRIX_H
#define NEUROFITTER_DIRECTVDVDTMATRIX_H

using namespace std;

#include "MapVdVdtMatrix.h"

class DirectVdVdtMatrix : public MapVdVdtMatrix {

public:
	DirectVdVdtMatrix();
	DirectVdVdtMatrix(const FixedParameters);
	DirectVdVdtMatrix(const DataTrace&, const FixedParameters);

	double compare(const VdVdtMatrix&) const;

};
	
#endif
