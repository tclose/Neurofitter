#include "../DistVdVdtMatrix.h"

DistVdVdtMatrix::DistVdVdtMatrix() 
	: MapVdVdtMatrix() {}

DistVdVdtMatrix::DistVdVdtMatrix(FixedParameters params) 
	: MapVdVdtMatrix(params) {}

DistVdVdtMatrix::DistVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	: MapVdVdtMatrix(trace, params) {}

double DistVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const DistVdVdtMatrix & other = dynamic_cast<const DistVdVdtMatrix &>(o);

	double fitnessValue = 0;
   
	if (other.getVLength() != vLength) crash("DistVdVdtMatrix","V dimensions don't match");
	if (other.getdVdtLength() != dVdtLength) crash("DistVdVdtMatrix","dVdt dimensions don't match");	

	for (map< const int, double >::const_iterator i = matrix.begin(); i != matrix.end(); i++) {
		for (map< const int, double >::const_iterator j = (other.matrix).begin(); j != (other.matrix).end(); j++) {
			fitnessValue += pow(i->second-j->second,2)*(pow((double)(vIndex(i->first)-other.vIndex(j->first)),2)+pow((double)(dVdtIndex(i->first)-other.dVdtIndex(j->first)),2));		
		}
  	}	

	return sqrt(fitnessValue);
}
