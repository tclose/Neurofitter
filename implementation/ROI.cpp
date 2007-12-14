#include <string>

#include "../Tools.h"
#include "../ROI.h"

ROI::~ROI() {
}

ROI::ROI(double newMinV, double newMaxV, double newMinVdVdt, double newMaxVdVdt, double newWeight) : minV(newMinV), maxV(newMaxV), minVdVdt(newMinVdVdt), maxVdVdt(newMaxVdVdt), weight(newWeight) {}


ROI::ROI(const string vBoundsString, const string dVdtBoundsString, const string weightString) {
	istringstream vBoundsStream(vBoundsString);
	vBoundsStream >> minV;
	vBoundsStream >> maxV;

	istringstream dVdtBoundsStream(dVdtBoundsString);
	dVdtBoundsStream >> minVdVdt;
	dVdtBoundsStream >> maxVdVdt;

	istringstream weightStream(vBoundsString);
	weightStream >> weight;
}

string ROI::toString() const {
	string returnString = "";
	returnString += "[" + str(minV) + " " + str(maxV) + "] ";
	returnString += "[" + str(minVdVdt) + " " + str(maxVdVdt) + "] ";
	returnString += "[" + str(weight) + "]";

	return returnString;
}

double ROI::getMinV() const {
	return minV;
}
        
double ROI::getMaxV() const {
	return maxV;
}
       
double ROI::getMinVdVdt() const {
	return minVdVdt;
}

double ROI::getMaxVdVdt() const {
	return maxVdVdt;
}
        
double ROI::getWeight() const {
	return weight;
}

