/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_MESHVDVDTMATRIX_H
#define NEUROFITTER_MESHVDVDTMATRIX_H

using namespace std;

#include "FixedParamObject.h"
#include "TracesReader.h"
#include "VdVdtMatrix.h"
#include "ROI.h"
#include "Tools.h"

class MeshVdVdtMatrix : public VdVdtMatrix {

public:
	MeshVdVdtMatrix();
	MeshVdVdtMatrix(FixedParameters params);

	virtual ~MeshVdVdtMatrix() {};

	int getVLength() const;
	int getdVdtLength() const;

	void setVdVdtLength(int newVLength, int newdVdtLength);

	void setMinMaxV(double newMinimalV, double newMaximalV);
	void setMinMaxdVdt(double newMinimaldVdt, double newMaximaldVdt);

	void setOverflow(bool);

	void setToROI(ROI roi);

	virtual double get(const int v, const int dVdt) const = 0;
	virtual void set(const int v, const int dVdt, double value) = 0;

	void readFrom(const DataTrace& trace);

	string toString() const;
	string toFileExportString() const;
	
protected:
	int vLength;
	int dVdtLength;	
	bool vdVdtLengthSet;
	double minimalV, maximalV, minimaldVdt, maximaldVdt;
	bool vSet, dVdtSet;
	bool overflow;

};
	
#endif
