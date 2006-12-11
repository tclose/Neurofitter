/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_DISTANCEVDVDTMATRIX_H
#define NEUROFITTER_DISTANCEVDVDTMATRIX_H

#include <iostream>
#include <cmath>

using namespace std;

#include "MapVdVdtMatrix.h"

class DistanceVdVdtMatrix : public MapVdVdtMatrix {

public:
	DistanceVdVdtMatrix();
	DistanceVdVdtMatrix(const FixedParameters);
	DistanceVdVdtMatrix(const DataTrace&, const FixedParameters);

	double compare(const VdVdtMatrix&) const;

};
	
#endif
