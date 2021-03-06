/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MAPVDVDTMATRIX_H
#define NEUROFITTER_MAPVDVDTMATRIX_H

#include <iostream>
#include <cmath>

#include "MeshVdVdtMatrix.h"

using namespace std;

#include "VdVdtMatrix.h"

class MapVdVdtMatrix : public MeshVdVdtMatrix {

public:
	MapVdVdtMatrix();
	virtual ~MapVdVdtMatrix();

	MapVdVdtMatrix(const FixedParameters);
	MapVdVdtMatrix(const DataTrace&, const FixedParameters);

	void makeEmpty();

	double get(const int v, const int dVdt) const;	
	void set(const int v, const int dVdt, double value);	

	virtual double compare(const VdVdtMatrix&) const = 0;
	
protected:


	map< int, double > matrix;

	int mIndex(const int v, const int dVdt) const;
	int vIndex(const int mIndex) const;
	int dVdtIndex(const int mIndex) const;	

};
	
#endif
