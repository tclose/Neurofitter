#include <string>

#include "../Tools.h"
#include "../ROI.h"

ROI::~ROI() {
}

ROI::ROI(int newRun, int newPeriod, int newRecordingsite, double newMinV, double newMaxV, double newMindVdt, double newMaxdVdt, int newVLength, int newdVdtLength, bool newOverflow, double newWeight) : 
	run(newRun),
	period(newPeriod),
	recordingsite(newRecordingsite),
	minV(newMinV), 
	maxV(newMaxV), 
	mindVdt(newMindVdt), 
	maxdVdt(newMaxdVdt),
	dVdtSet(true),
	vLength(newVLength),
	dVdtLength(newdVdtLength),
	overflow(newOverflow), 
	weight(newWeight) {}


ROI::ROI(int newRun, int newPeriod, int newRecordingsite, const string vBoundsString, const string dVdtBoundsString, const string lengthsString, const string overflowString, const string weightString) : 
	run(newRun),
    period(newPeriod),
    recordingsite(newRecordingsite),
	dVdtSet(false)
 {
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
	int overflowInt;
	overflowStream >> overflowInt;

	if (overflowInt == 0) overflow = false;
	else if (overflowInt == 1) overflow = true;
	else crash("ROI", "UseOverflow can't be parsed, use 1 for true, 0 for false");

	istringstream weightStream(weightString);
	weightStream >> weight;
}

string ROI::toString() const {
	string returnString = "";
	returnString += "[" + str(run) + "] ";
	returnString += "[" + str(period) + "] ";
	returnString += "[" + str(recordingsite) + "] ";
	returnString += "[" + str(minV) + " " + str(maxV) + "] ";
	if (dVdtSet) {
		returnString += "[" + str(mindVdt) + " " + str(maxdVdt) + "] ";
	}
	else returnString += "[]";
	returnString += "[" + str(vLength) + " " + str(dVdtLength) + "] ";
	if (overflow) returnString += "[ 1 ] ";
	else returnString += "[ 0 ] ";
	returnString += "[" + str(weight) + "]";

	return returnString;
}

int ROI::getRun() const {
	return run;
}

int ROI::getPeriod() const {
	return period;
}

int ROI::getRecordingsite() const {
	return recordingsite;
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
