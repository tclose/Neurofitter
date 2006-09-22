#ifndef NEUROFITTER_DISTVDVDTMATRIX_H
#define NEUROFITTER_DISTVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "DataTrace.h"
#include "MapVdVdtMatrix.h"
#include "FixedParamObject.h"
#include "Tools.h"

class DistVdVdtMatrix : public MapVdVdtMatrix {

public:
	DistVdVdtMatrix();
	DistVdVdtMatrix(const FixedParameters);
	DistVdVdtMatrix(const DataTrace&, const FixedParameters);

	double compare(const VdVdtMatrix&) const;

};
	
#endif
