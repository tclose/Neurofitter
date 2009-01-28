/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../MeshVdVdtMatrix.h"

MeshVdVdtMatrix::MeshVdVdtMatrix() : VdVdtMatrix(), vLength(), dVdtLength(), vdVdtLengthSet(false), vSet(false), dVdtSet(false), overflow(true) {

} 

MeshVdVdtMatrix::MeshVdVdtMatrix(FixedParameters params) : 
		VdVdtMatrix(params),
		vdVdtLengthSet(false),
		vSet(false), 
		dVdtSet(false),
		overflow(true) {
		
		if (fixedParams.parameterExists("vLength") && fixedParams.parameterExists("dVdtLength")) setVdVdtLength(toInt(fixedParams["vLength"]), toInt(fixedParams["dVdtLength"]));

}	

int MeshVdVdtMatrix::getVLength() const {
	
	if (vdVdtLengthSet) return vLength; 
	else crash("MeshVdVdtMatrix","vLength not set"); 
	return -1;

}
	
int MeshVdVdtMatrix::getdVdtLength() const {

	if (vdVdtLengthSet) return dVdtLength; else crash("MeshVdVdtMatrix","vdVdtLength not set"); 
	return -1;

}

	
void MeshVdVdtMatrix::setVdVdtLength(int newVLength, int newdVdtLength) { 
	
	vLength = newVLength; 
	dVdtLength = newdVdtLength; 
	vdVdtLengthSet = true;

}

void MeshVdVdtMatrix::setMinMaxV(double newMinimalV, double newMaximalV) {
	
	minimalV = newMinimalV; 
	maximalV = newMaximalV; 
	vSet = true;

}
	
void MeshVdVdtMatrix::setMinMaxdVdt(double newMinimaldVdt, double newMaximaldVdt) {
	
	minimaldVdt = newMinimaldVdt; 
	maximaldVdt = newMaximaldVdt; 
	dVdtSet = true;

}

void MeshVdVdtMatrix::setOverflow(bool newOverflow) {

	overflow = newOverflow;
	
}

void MeshVdVdtMatrix::setToROI(ROI roi) {
		
	setMinMaxV(roi.getMinV(), roi.getMaxV());
	if (roi.dVdtIsSet()) setMinMaxdVdt(roi.getMindVdt(), roi.getMaxdVdt());
	setVdVdtLength(roi.getVLength(), roi.getdVdtLength());
	setOverflow(roi.getOverflow());

}


void MeshVdVdtMatrix::readFrom(const DataTrace& trace) {
	
	makeEmpty();

	//////////////////
    /// Initialize ///
    //////////////////

    double dVdt = 0;
    double V = 0, VPrev = 0, VNext = 0;

	if (!vSet) { 
    	minimalV = toDouble(fixedParams["minimalV"]); // lowest possible V values in VdVdt matrix
    	maximalV = toDouble(fixedParams["maximalV"]); // highest possible V values in VdVdt matrix
	}

	if (!dVdtSet) {
    	minimaldVdt = -(maximalV-minimalV)*trace.getSamplingFrequency();
    	maximaldVdt = (maximalV-minimalV)*trace.getSamplingFrequency();
	}

    const double dxVdVdtmatrix = (maximalV-minimalV)/vLength;
    const double dyVdVdtmatrix = (maximaldVdt-minimaldVdt)/dVdtLength;

    ///////////////////////////////////////
    /// Fill the matrix (and normalize) ///
    ///////////////////////////////////////

	this->setMaxNumberOfPoints(trace.getValidLength()-2*trace.getLag());

    for (int nPoint = trace.getLag(); nPoint < trace.getLength()-trace.getLag(); nPoint++) {
		if (trace.pointIsValid(nPoint) && trace.pointIsValid(nPoint-trace.getLag()) && trace.pointIsValid(nPoint+trace.getLag())) {
       		V = trace.get(nPoint);
			// Multiplied by 2 to normalize to 1 of weight = 1/2
       		VPrev = 2*(1-trace.getLagWeight())*trace.get(nPoint-trace.getLag());
       		VNext = 2*(trace.getLagWeight())*trace.get(nPoint+trace.getLag());
       		dVdt = (VPrev-VNext) * trace.getSamplingFrequency();

        	//if (V < minimalV) showMessage("Warning: V smaller than minimal V in MapVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
        	//if (V >= maximalV) showMessage("Warning: V larger than maximal V in MapVdVdtMatrix: " + str(V) + "\n",5,fixedParams);
        	//if (dVdt < minimaldVdt) showMessage("Warning: dVdt smaller than minimal dVdt in MapVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);
        	//if (dVdt >= maximaldVdt) showMessage("Warning: dVdt larger than maximal dVdt in MapVdVdtMatrix: " + str(dVdt) + "\n",5,fixedParams);

        	int vIndex = (int)( (V-minimalV) / dxVdVdtmatrix );
        	int dVdtIndex = (int)( (dVdt-minimaldVdt) / dyVdVdtmatrix );

			if (overflow) {
        		if (V < minimalV) vIndex = 0;
        		if (V >= maximalV) vIndex = vLength-1;
        		if (dVdt < minimaldVdt) dVdtIndex = 0;
        		if (dVdt >= maximaldVdt) dVdtIndex = dVdtLength-1;
			}
			else {
        		if (V < minimalV || V >= maximalV || dVdt < minimaldVdt || dVdt >= maximaldVdt) continue;
			}

			//if (!overflow) showMessage(str(vIndex) + " " + str(dVdtIndex) + " " + str(V) + " " + str(minimalV) + " " + str(dxVdVdtmatrix) + "\n");

			set(vIndex,dVdtIndex,get(vIndex,dVdtIndex) + 1); //1.0/(trace.getValidLength()-2*trace.getLag())
		}
		else {
			showMessage("Warning: Data point with seq number "+str(nPoint)+" is not valid in the defined time range of the Datatrace object: "+trace.getName()+"\n",15,fixedParams);
		}
	}

}

string MeshVdVdtMatrix::toString() const {

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
        
    return result.str();

}

string MeshVdVdtMatrix::toFileExportString() const {

    ostringstream result;

	result << dVdtLength << " " << vLength << " ";
    for (int dVdtIndex=0;dVdtIndex<dVdtLength;dVdtIndex++) {
		for (int vIndex=0;vIndex<vLength;vIndex++) {
			result << " " << get(vIndex, dVdtIndex);
		}
	}
        
    return result.str();
}
