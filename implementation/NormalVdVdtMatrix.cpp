#include <iostream>
#include <cmath>

#include "../Tools.h"

#include "../NormalVdVdtMatrix.h"

NormalVdVdtMatrix::NormalVdVdtMatrix() 
	: VdVdtMatrix(), matrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {}

NormalVdVdtMatrix::NormalVdVdtMatrix(FixedParameters params) 
	: VdVdtMatrix(params), matrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {}

NormalVdVdtMatrix::NormalVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	VdVdtMatrix(params), 
		matrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {

	//////////////////
	/// Initialize ///
	//////////////////

    double dVdt = 0;
    double V = 0, VPrev = 0, VNext = 0;

	const double minimalV = toDouble(fixedParams["minimalV"]); // lowest possible V values in VdVdt matrix
    const double maximalV = toDouble(fixedParams["maximalV"]); // highest possible V values in VdVdt matrix

	const double minimaldVdt = -(maximalV-minimalV)*trace.getSamplingFrequency();
	const double maximaldVdt = (maximalV-minimalV)*trace.getSamplingFrequency();

    const double dxVdVdtmatrix = (maximalV-minimalV)/vLength; 
    const double dyVdVdtmatrix = (maximaldVdt-minimaldVdt)/dVdtLength;

	///////////////////////////////////////
	/// Fill the matrix (and normalize) ///
	///////////////////////////////////////

    for (int nPoint = 1; nPoint < trace.getLength()-1; nPoint++) {
		V = trace[nPoint];
		VPrev = trace[nPoint-1];
		VNext = trace[nPoint+1];
		dVdt = (VPrev-VNext) * trace.getSamplingFrequency();

		if (V < minimalV) showMessage("Warning: V smaller than minimal V in NormalVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
		if (V >= maximalV) showMessage("Warning: V larger than maximal V in NormalVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
		if (dVdt < minimaldVdt) showMessage("Warning: dVdt smaller than minimal dVdt in NormalVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);
		if (dVdt >= maximaldVdt) showMessage("Warning: dVdt larger than maximal dVdt in NormalVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);

		if (V < minimalV) V=minimalV;
		if (V >= maximalV) V=maximalV-dxVdVdtmatrix; 
		if (dVdt < minimaldVdt) dVdt=minimaldVdt;
		if (dVdt >= maximaldVdt) dVdt=maximaldVdt-dyVdVdtmatrix;

		int vIndex = (int)( (V-minimalV) / dxVdVdtmatrix );
		int dVdtIndex = (int)( (dVdt-minimaldVdt) / dyVdVdtmatrix );

		set(vIndex, dVdtIndex, get(vIndex,dVdtIndex) + 1.0/(trace.getLength()-2));
	}
}

double NormalVdVdtMatrix::compare(const VdVdtMatrix & other) const {

	double fitnessValue = 0;

	double diff = 0;

  	const double precision = toDouble(fixedParams["comparePrecision"]);
   
	if (other.getVLength() != vLength) crash("NormalVdVdtMatrix","V dimensions don't match");
	if (other.getdVdtLength() != dVdtLength) crash("NormalVdVdtMatrix","dVdt dimensions don't match");	

	///////////////////////////////////////////////////////////
	/// Calculate the square root of the sum of the squares ///
	///////////////////////////////////////////////////////////

	for (int vIndex=0;vIndex<vLength;vIndex++) {
    	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
			diff=fabs(get(vIndex,dVdtIndex)-other.get(vIndex,dVdtIndex));
			if (diff > precision) {
				fitnessValue += pow(diff,2);
			}
		}
   	}

	return sqrt(fitnessValue);
}


void NormalVdVdtMatrix::set(const int v, const int dVdt, const double value) {
    if (v < 0 || v >= vLength) {
		crash("NormalVdVdtMatrix","Invalid v: "+str(v));
	}
    if (dVdt < 0 || dVdt >= dVdtLength) {
		crash("NormalVdVdtMatrix","Invalid dVdt: "+str(dVdt));
	}
	matrix[v][dVdt] = value;
}

const double NormalVdVdtMatrix::get(const int v, const int dVdt) const {
    if (v < 0 || v >= vLength) {
		crash("NormalVdVdtMatrix","Invalid v: "+str(v));
	}
    if (dVdt < 0 || dVdt >= dVdtLength) {
		crash("NormalVdVdtMatrix","Invalid dV: "+str(dVdt));
	}
	return matrix[v][dVdt];
}
