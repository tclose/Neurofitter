/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_ROIVDVDTMATRIX_H
#define NEUROFITTER_ROIVDVDTMATRIX_H

using namespace std;

#include "FixedParamObject.h"
#include "ROIStringParser.h"
#include "TracesReader.h"
#include "VdVdtMatrix.h"
#include "DirectVdVdtMatrix.h"
#include "Tools.h"

class ROIVdVdtMatrix : public VdVdtMatrix {

public:
	ROIVdVdtMatrix(); 
	ROIVdVdtMatrix(FixedParameters params);	
	ROIVdVdtMatrix(DataTrace & trace, FixedParameters params);	

	void makeEmpty();

	virtual ~ROIVdVdtMatrix() {};

	double compare(const VdVdtMatrix&) const;

	void readFrom(const DataTrace& trace);


	string toString() const;

	
protected:
	///todo Make this more generic (no directvdvdt)
	vector< DirectVdVdtMatrix > matrices;
	ROIStringParser ROIsParser;


};
	
#endif
