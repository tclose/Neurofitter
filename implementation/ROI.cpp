#include <string>

#include "../Tools.h"
#include "../ROI.h"

ROI::~ROI() {
}

ROI::ROI(double newMinV, double newMaxV, double newMindVdt, double newMaxdVdt, int newVLength, int newdVdtLength, bool newOverflow, double newWeight) : 
	minV(newMinV), 
	maxV(newMaxV), 
	mindVdt(newMindVdt), 
	maxdVdt(newMaxdVdt),
	dVdtSet(true),
	vLength(newVLength),
	dVdtLength(newdVdtLength),
	overflow(newOverflow), 
	weight(newWeight) {}


ROI::ROI(const string vBoundsString, const string dVdtBoundsString, const string lengthsString, const string overflowString, const string weightString) : dVdtSet(false) {
	istringstream vBoundsStream(vBoundsString);
	vBoundsStream >> minV;
	vBoundsStream >> maxV;

	if (!(trim(dVdtBoundsString) == "")) {
		istringstream dVdtBoundsStream(dVdtBoundsString);
		dVdtBoundsStream >> mindVdt;
		dVdtBoundsStream >> maxdVdt;
		dVdtSet = true;
	}

	istringstream lengthsStream(lengthsString);
	lengthsStream >> vLength;
	lengthsStream >> dVdtLength;

	istringstream overflowStream(overflowString);
	overflowStream >> overflow;

	istringstream weightStream(weightString);
	weightStream >> weight;
}

string ROI::toString() const {
	string returnString = "";
	returnString += "[" + str(minV) + " " + str(maxV) + "] ";
	if (dVdtSet) {
		returnString += "[" + str(mindVdt) + " " + str(maxdVdt) + "] ";
	}
	else returnString += "[]";
	returnString += "[" + str(vLength) + " " + str(dVdtLength) + "] ";
	returnString += "[" + str(overflow) + "] ";
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

bool ROI::getOverflow() const {
	return overflow;
}

double ROI::getWeight() const {
	return weight;
}

bool ROI::dVdtIsSet() const {
	return dVdtSet;
}
