#ifndef NEUROFITTER_VDVDTMATRIX_H
#define NEUROFITTER_VDVDTMATRIX_H

using namespace std;

#include "FixedParamObject.h"
#include "Tools.h"

class VdVdtMatrix : public FixedParamObject {

public:
	VdVdtMatrix() : FixedParamObject(), vLength(), dVdtLength() {}; 
	VdVdtMatrix(FixedParameters params) : 
		FixedParamObject(params),
        vLength(toInt(fixedParams["vLength"])),
        dVdtLength(toInt(fixedParams["dVdtLength"])) {};	

	virtual ~VdVdtMatrix() {};

	int getVLength() const { return vLength; };
	int getdVdtLength() const { return dVdtLength; };

	virtual const double get(const int v, const int dVdt) const = 0;
	virtual void set(const int v, const int dVdt, double value) = 0;

	virtual double compare(const VdVdtMatrix&) const = 0;

	virtual void makeEmpty() = 0;

	void readFrom(const DataTrace& trace) {
		makeEmpty();

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

        	if (V < minimalV) showMessage("Warning: V smaller than minimal V in MapVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
        	if (V >= maximalV) showMessage("Warning: V larger than maximal V in MapVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
        	if (dVdt < minimaldVdt) showMessage("Warning: dVdt smaller than minimal dVdt in MapVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);
        	if (dVdt >= maximaldVdt) showMessage("Warning: dVdt larger than maximal dVdt in MapVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);

        	if (V < minimalV) V=minimalV;
        	if (V >= maximalV) V=maximalV-dxVdVdtmatrix;                                                    
        	if (dVdt < minimaldVdt) dVdt=minimaldVdt;
        	if (dVdt >= maximaldVdt) dVdt=maximaldVdt-dyVdVdtmatrix;

        	int vIndex = (int)( (V-minimalV) / dxVdVdtmatrix );
        	int dVdtIndex = (int)( (dVdt-minimaldVdt) / dyVdVdtmatrix );

			set(vIndex,dVdtIndex,get(vIndex,dVdtIndex) + 1.0/(trace.getLength()-2));
		}

	}

	string toString() const {
    	ostringstream result;
    	double value;

    	bool numeric = toInt(fixedParams["numericOutputFormat"]);

    	for (int vIndex=0;vIndex<vLength;vIndex++) {
        	for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
            	value = get(vIndex,dVdtIndex);
            	if (numeric) {
                	 result << " " << value;
            	}
            	else {
                	if (value == 0) result << " .";
                	else result << " #";
            	}
        }
        result << endl;
    }
        
    return result.str();
}


	
protected:
	int vLength;
	int dVdtLength;	

};
	
#endif
