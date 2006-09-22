#ifndef NEUROFITTER_NORMALVDVDTMATRIX_H
#define NEUROFITTER_NORMALVDVDTMATRIX_H

using namespace std;

#include "DataTrace.h"
#include "VdVdtMatrix.h"

class NormalVdVdtMatrix : public VdVdtMatrix  {

public:
	NormalVdVdtMatrix();
	NormalVdVdtMatrix(const FixedParameters);
	NormalVdVdtMatrix(const DataTrace&, const FixedParameters);

	int getVLength() const;
	int getdVdtLength() const;

	virtual void set(const int v, const int dVdt, const double value);
	virtual const double get(const int v, const int dVdt) const;

	void makeEmpty();

	virtual double compare(const VdVdtMatrix&) const;
	
protected:
	map< int, double > matrix;

private:    
	int mIndex(const int v, const int dVdt) const;
   	int vIndex(const int mIndex) const;
    int dVdtIndex(const int mIndex) const;


};
	
#endif
