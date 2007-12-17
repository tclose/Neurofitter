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

	void readFrom(const DataTrace& trace) {
		makeEmpty();


		
		/*
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

    	for (int nPoint = trace.getLag(); nPoint < trace.getLength()-trace.getLag(); nPoint++) {
			if (trace.pointIsValid(nPoint) && trace.pointIsValid(nPoint-trace.getLag()) && trace.pointIsValid(nPoint+trace.getLag())) {
        		V = trace.get(nPoint);
				// Multiplied by 2 to normalize to 1 of weight = 1/2
        		VPrev = 2*(1-trace.getLagWeight())*trace.get(nPoint-trace.getLag());
        		VNext = 2*(trace.getLagWeight())*trace.get(nPoint+trace.getLag());
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

				set(vIndex,dVdtIndex,get(vIndex,dVdtIndex) + 1.0/(trace.getValidLength()-2*trace.getLag()));
			}
			else {
				showMessage("Warning: Not all of data points in the defined time range are valid in the Datatrace object\n",15,fixedParams);
			}
		}
		*/
	}

	string toString() const {
	/*
    	ostringstream result;
    	double value;

    	bool numeric = toInt(fixedParams["numericOutputFormat"]);

        for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
    		for (int vIndex=0;vIndex<vLength;vIndex++) {
            	value = get(vIndex,dVdtIndex);
            	if (numeric) {
                	 result << " " << value;
            	}
            	else {
                	if (value < toDouble(fixedParams["comparePrecision"])) result << " .";
                	else result << " #";
            	}
        }
        result << endl;
    }
        
    return result.str(); */ 
	return "ROIVdVdtMatrix";
}


	
protected:
	///todo Make this more generic (no directvdvdt)
	vector< DirectVdVdtMatrix > matrices;
	ROIStringParser ROIs;


};
	
#endif
