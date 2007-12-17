#include <string>

#include "../Tools.h"
#include "../ROI.h"

ROI::~ROI() {
}

ROI::ROI(double newMinV, double newMaxV, double newMindVdt, double newMaxdVdt, int newVLength, int newdVdtLength, double newWeight) : 
	minV(newMinV), 
	maxV(newMaxV), 
	mindVdt(newMindVdt), 
	maxdVdt(newMaxdVdt),
	vLength(newVLength),
	dVdtLength(newdVdtLength), 
	weight(newWeight) {}


ROI::ROI(const string vBoundsString, const string dVdtBoundsString, const string lengthsString, const string weightString) {
	istringstream vBoundsStream(vBoundsString);
	vBoundsStream >> minV;
	vBoundsStream >> maxV;

	istringstream dVdtBoundsStream(dVdtBoundsString);
	dVdtBoundsStream >> mindVdt;
	dVdtBoundsStream >> maxdVdt;

	istringstream lengthsStream(lengthsString);
	lengthsStream >> vLength;
	lengthsStream >> dVdtLength;

	istringstream weightStream(vBoundsString);
	weightStream >> weight;
}

string ROI::toString() const {
	string returnString = "";
	returnString += "[" + str(minV) + " " + str(maxV) + "] ";
	returnString += "[" + str(mindVdt) + " " + str(maxdVdt) + "] ";
	returnString += "[" + str(vLength) + " " + str(dVdtLength) + "] ";
	returnString += "[" + str(weight) + "]";

	return returnString;
}

double ROI::getMinV() const {
	return minV;
}
        
double ROI::getMaxV() const {
	return maxV;
}
       
double ROI::getMindVdt() const {
	return mindVdt;
}

double ROI::getMaxdVdt() const {
	return maxdVdt;
}

int ROI::getVLength() const {
	return vLength;
}
        
int ROI::getdVdtLength() const {
	return dVdtLength;
}

double ROI::getWeight() const {
	return weight;
}

