#include "../NormalVdVdtMatrix.h"

NormalVdVdtMatrix::NormalVdVdtMatrix() 
	: FixedParamObject(), vLength(0), dVdtLength(0), VdVdtMatrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {}

NormalVdVdtMatrix::NormalVdVdtMatrix(FixedParameters params) 
	: FixedParamObject(params), vLength(0), dVdtLength(0), VdVdtMatrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {}

NormalVdVdtMatrix::NormalVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	:	FixedParamObject(params), 
		vLength(toInt(fixedParams["vLength"])), 
		dVdtLength(toInt(fixedParams["dVdtLength"])), 
		VdVdtMatrix(vector< vector< double > >(vLength,vector< double >(dVdtLength))) {

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

		(*this)[vIndex][dVdtIndex] += 1.0/(trace.getLength()-2);
	}
}

int NormalVdVdtMatrix::getVLength() const {
	return vLength;
}

int NormalVdVdtMatrix::getdVdtLength() const {
	return dVdtLength;
}

double NormalVdVdtMatrix::compare(const NormalVdVdtMatrix & other) const {

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
			diff=fabs((*this)[vIndex][dVdtIndex]-other[vIndex][dVdtIndex]);
			if (diff > precision) {
				fitnessValue += pow(diff,2);
			}
		}
   	}

	return sqrt(fitnessValue);
}


inline vector<double>& NormalVdVdtMatrix::operator[] (const int subscript) {
    if (subscript < 0 || subscript >= vLength) {
		crash("NormalVdVdtMatrix","Invalid subscript: "+str(subscript));
	}
	return VdVdtMatrix[subscript];
}


inline const vector<double>& NormalVdVdtMatrix::operator[] (const int subscript) const {
    if (subscript < 0 || subscript >= vLength) {
		crash("NormalVdVdtMatrix","Invalid subscript: "+str(subscript));
	}
	return VdVdtMatrix[subscript];
}

string NormalVdVdtMatrix::toString() const {
	ostringstream result;

	for (int vIndex=0;vIndex<vLength;vIndex++) {
        for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            result << " " << (*this)[vIndex][dVdtIndex];
        }
		result << endl;
    }

	return result.str();
}
