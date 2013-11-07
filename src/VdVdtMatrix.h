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
	VdVdtMatrix() : FixedParamObject(), weightSet(false), maxNumberOfPoints(-1) {}; 
	VdVdtMatrix(FixedParameters params) : FixedParamObject(params), weightSet(false), maxNumberOfPoints(-1) {};	

	virtual ~VdVdtMatrix() {};

	virtual double compare(const VdVdtMatrix&) const = 0;

	virtual void readFrom(const DataTrace& trace) = 0;

	virtual string toString() const = 0;

	virtual string toFileExportString() const = 0;

	virtual void makeEmpty() = 0;

	double getWeight() const {if (!weightSet) crash("VdVdtMatrix","Weight not set"); return weight;};
	
	void setWeight(double newWeight) {weight = newWeight; weightSet = true;}

	void setMaxNumberOfPoints(int nOfPoints) {maxNumberOfPoints = nOfPoints;}

	int getMaxNumberOfPoints() const {
		if (maxNumberOfPoints == -1) {
			crash("VdVdtMatrix","Max number of points not set"); 
			return -1;
		}
		else {
			return maxNumberOfPoints;
		}
	}

	protected:
		bool weightSet;
		double weight;
		int maxNumberOfPoints;

};
	
#endif
