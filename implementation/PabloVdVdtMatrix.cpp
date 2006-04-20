#include "../PabloVdVdtMatrix.h"

///todo make this object use maps

PabloVdVdtMatrix::PabloVdVdtMatrix() 
	: FixedParamObject(), VdVdtMatrix(NULL), vLength(0), dVdtLength(0) {}

PabloVdVdtMatrix::PabloVdVdtMatrix(FixedParameters params) 
	: FixedParamObject(params), VdVdtMatrix(NULL), vLength(0), dVdtLength(0) {}

PabloVdVdtMatrix& PabloVdVdtMatrix::operator=(const PabloVdVdtMatrix& p) {

	if (this != &p) {
		if (VdVdtMatrix != NULL) {
        	for (int i = 0; i < vLength; i++) {
				if (VdVdtMatrix[i] != NULL) {delete [] VdVdtMatrix[i];}
        	}
        	delete [] VdVdtMatrix;
    	}
		vLength = p.vLength;
		dVdtLength = p.dVdtLength;
		VdVdtMatrix = new double * [vLength];
		if (VdVdtMatrix == NULL) {cerr << endl <<"Error: Unable to allocate array in PabloVdVdtMatrix" << endl;exit(1);}
		for (int i = 0; i < vLength; i++) {
			VdVdtMatrix[i] = new double [dVdtLength];
			if (VdVdtMatrix[i] == NULL) {cerr << endl <<"Error: Unable to allocate array in PabloVdVdtMatrix" << endl;exit(1);}
			for (int j = 0; j < dVdtLength; j++) {
				(*this)[i][j] = p[i][j];
			}
		}		
	}

	return *this;
}

PabloVdVdtMatrix::PabloVdVdtMatrix(const DataTrace& trace, FixedParameters params) :
	 FixedParamObject(params)  {

	//////////////////
	/// Initialize ///
	//////////////////

    double dVdt = 0;
    double V = 0, VPrev = 0, VNext = 0;

    vLength=toInt(fixedParams["vLength"]); /// Number of indices in the V direction in VdVdt matrix
    dVdtLength=toInt(fixedParams["dVdtLength"]); /// Number of indices in the dVdt direction in VdVdt matrix

	const double minimalV = toDouble(fixedParams["minimalV"]); // lowest possible V values in VdVdt matrix
    const double maximalV = toDouble(fixedParams["maximalV"]); // highest possible V values in VdVdt matrix

	const double minimaldVdt = -(maximalV-minimalV)*trace.getSamplingFrequency();
	const double maximaldVdt = (maximalV-minimalV)*trace.getSamplingFrequency();

    const double dxVdVdtmatrix = (maximalV-minimalV)/vLength; 
    const double dyVdVdtmatrix = (maximaldVdt-minimaldVdt)/dVdtLength;

    VdVdtMatrix = new double * [vLength];
	if (VdVdtMatrix == NULL) {cerr << endl <<"Error: Unable to allocate array in PabloVdVdtMatrix" << endl;exit(1);}
	for (int i = 0; i < vLength; i++) {
		VdVdtMatrix[i] = new double [dVdtLength];
		if (VdVdtMatrix[i] == NULL) {cerr << endl <<"Error: Unable to allocate array in PabloVdVdtMatrix" << endl;exit(1);}
		for (int j = 0; j < dVdtLength; j++) {
			(*this)[i][j] = 0;
		}

	}		

	///////////////////////////////////////
	/// Fill the matrix (and normalize) ///
	///////////////////////////////////////

    for (int nPoint = 1; nPoint < trace.getLength()-1; nPoint++) {
		V = trace[nPoint];
		VPrev = trace[nPoint-1];
		VNext = trace[nPoint+1];
		dVdt = (VPrev-VNext) * trace.getSamplingFrequency();

		if (toInt(fixedParams["VerboseLevel"]) > 4) {	
			if (V < minimalV) {cerr << "Warning: V smaller than minimal V in PabloVdVdtMatrix: "<<str(V)<<endl;}
			if (V >= maximalV) {cerr << "Warning: V larger than maximal V in PabloVdVdtMatrix: "<<str(V)<<endl;}
			if (dVdt < minimaldVdt) {cerr << "Warning: dVdt smaller than minimal dVdt in PabloVdVdtMatrix: "<<str(dVdt)<<endl;}
			if (dVdt >= maximaldVdt) {cerr << "Warning: dVdt larger than maximal dVdt in PabloVdVdtMatrix: "<<str(dVdt)<<endl;}
		}

		if (V < minimalV) {V=minimalV;}
		if (V >= maximalV) {V=maximalV-dxVdVdtmatrix;} ///todo check if in extreme cases this cannot cause problems
		if (dVdt < minimaldVdt) {dVdt=minimaldVdt;}
		if (dVdt >= maximaldVdt) {dVdt=maximaldVdt-dyVdVdtmatrix;}

		int vIndex = (int)( (V-minimalV) / dxVdVdtmatrix );
		int dVdtIndex = (int)( (dVdt-minimaldVdt) / dyVdVdtmatrix );

		(*this)[vIndex][dVdtIndex] += 1.0/(trace.getLength()-2);
	}
}

PabloVdVdtMatrix::~PabloVdVdtMatrix() {

	if (VdVdtMatrix != NULL) {
		for (int i = 0; i < vLength; i++) {
			if (VdVdtMatrix[i] != NULL) {delete [] VdVdtMatrix[i];}
		}		
		delete [] VdVdtMatrix;
	}
}


int PabloVdVdtMatrix::getVLength() const {
	return vLength;
}

int PabloVdVdtMatrix::getdVdtLength() const {
	return dVdtLength;
}


double PabloVdVdtMatrix::compare(const PabloVdVdtMatrix & other) const {

	double fitnessValue = 0;

	double diff = 0;

  	const double precision = toDouble(fixedParams["comparePrecision"]);
   
	if (other.getVLength() != vLength) {cerr << endl << "Error: V dimensions don't match in PabloVdVdtMatrix" << endl;exit(1);} 	
	if (other.getdVdtLength() != dVdtLength) {cerr << endl << "Error: dVdt dimensions don't match in PabloVdVdtMatrix" << endl;exit(1);}	

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


inline double* PabloVdVdtMatrix::operator[] (int subscript) {
    if (0 <= subscript && subscript < vLength)
        {return VdVdtMatrix[subscript];}
    else
        {cerr << endl << "Error: Invalid subscript in PabloVdVdtMatrix: "<<subscript<<endl;exit(1);}
}


inline double const*const PabloVdVdtMatrix::operator[] (int subscript) const {
    if (0 <= subscript && subscript < vLength)
        {return VdVdtMatrix[subscript];}
    else
        {cerr << endl << "Error: Invalid subscript in PabloVdVdtMatrix: "<<subscript<<endl;exit(1);}
}


string PabloVdVdtMatrix::toString() const {

	ostringstream result;

	for (int vIndex=0;vIndex<vLength;vIndex++) {
        for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            result << " " << (*this)[vIndex][dVdtIndex];
        }
		result << endl;
    }

	return result.str();
}
