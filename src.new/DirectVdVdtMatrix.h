/*
Revision of last commit: $Rev: 162 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2006-12-11 18:44:13 +0900 (Mon, 11 Dec 2006) $
*/

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
