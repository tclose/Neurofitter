#include "../DistanceVdVdtMatrix.h"

DistanceVdVdtMatrix::DistanceVdVdtMatrix() 
	: MapVdVdtMatrix() {}

DistanceVdVdtMatrix::DistanceVdVdtMatrix(FixedParameters params) 
	: MapVdVdtMatrix(params) {}

DistanceVdVdtMatrix::DistanceVdVdtMatrix(const DataTrace& trace, FixedParameters params) 
	: MapVdVdtMatrix(trace, params) {}

double DistanceVdVdtMatrix::compare(const VdVdtMatrix & o) const {

	const DistanceVdVdtMatrix & other = dynamic_cast<const DistanceVdVdtMatrix &>(o);

	double fitnessValue = 0;
   
	if (other.getVLength() != vLength) crash("DistanceVdVdtMatrix","V dimensions don't match");
	if (other.getdVdtLength() != dVdtLength) crash("DistanceVdVdtMatrix","dVdt dimensions don't match");	

	double diff;
	double distance, v1, v2, dVdt1, dVdt2;
	double maxDistance = pow((double)vLength,2)+pow((double)dVdtLength,2);
	for (map< const int, double >::const_iterator i = matrix.begin(); i != matrix.end(); i++) {
		for (map< const int, double >::const_iterator j = (other.matrix).begin(); j != (other.matrix).end(); j++) {
			diff = i->second-j->second;
			v1 = vIndex(i->first);
			v2 = other.vIndex(j->first);
			dVdt1 = dVdtIndex(i->first);
			dVdt2 = other.dVdtIndex(j->first);
			distance = pow(v1-v2,2)+pow(dVdt1-dVdt2,2);
			fitnessValue += pow(diff,2)*pow(maxDistance-distance,2);
		}
  	}	

	return sqrt(fitnessValue);
}

