/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
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
