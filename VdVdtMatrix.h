/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_VDVDTMATRIX_H
#define NEUROFITTER_VDVDTMATRIX_H

using namespace std;

#include "FixedParamObject.h"
#include "TracesReader.h"
#include "Tools.h"

class VdVdtMatrix : public FixedParamObject {

public:
	VdVdtMatrix() : FixedParamObject() {}; 
	VdVdtMatrix(FixedParameters params) : FixedParamObject(params) {};	

	virtual ~VdVdtMatrix() {};

	virtual double compare(const VdVdtMatrix&) const = 0;

	virtual void makeEmpty() = 0;

	virtual void readFrom(const DataTrace& trace) = 0;

	virtual string toString() const = 0;

};
	
#endif
