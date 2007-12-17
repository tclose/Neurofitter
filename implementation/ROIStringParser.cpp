#include <string>
#include <sstream>

#include "../ROI.h"
#include "../Tools.h"
#include "../ROIStringParser.h"

ROIStringParser::~ROIStringParser() {}

ROIStringParser::ROIStringParser(const string newString, int newNumberOfROILines, int newNumberOfRuns, int newNumberOfPeriods) {

	initialize(newString, newNumberOfROILines, newNumberOfRuns, newNumberOfPeriods);

}


void ROIStringParser::initialize(const string newString, int newNumberOfROILines, int newNumberOfRuns, int newNumberOfPeriods) {

	ROIs.clear();
	sourceString=newString;
	numberOfROILines = newNumberOfROILines;
	numberOfRuns = newNumberOfRuns;
	numberOfPeriods = newNumberOfPeriods;
	numberOfROIs = 0;

	string dummy, allRunStr, allPeriodStr, runStr, periodStr, vBoundsStr, dVdtBoundsStr, lengthsStr, weightStr;
	int run, period;

	istringstream ss(sourceString);

	for (int i = 0; i < numberOfROILines; i++) {
		getline(ss, dummy, '['); 
		getline(ss, allRunStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, allPeriodStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, vBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, dVdtBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, lengthsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, weightStr, ']');

		istringstream runStream(allRunStr);
		istringstream periodStream(allPeriodStr);
		while (runStream.good()) {
			runStream >> run;
			if (run < 0 || run >= numberOfRuns) crash("ROIStringParser","Run with number " + str(run) + " does not exist");
			periodStream >> period;
			if (period < 0 || period>= numberOfPeriods) crash("ROIStringParser","Period with number " + str(period) + " does not exist");

			ROI roi(vBoundsStr, dVdtBoundsStr, lengthsStr, weightStr);
			ROIs[run*numberOfPeriods+period].push_back(roi);
			numberOfROIs++;			
		}		
		
	}

}




string ROIStringParser::toString() const {
	string returnString ="";
	returnString += "Number of runs: " + str(numberOfRuns) + "\tNumber of periods: " + str(numberOfPeriods) + "\tNumber of ROIs: " + str(numberOfROIs) + "\n";
	for (map< int , vector< ROI > >::const_iterator iter = ROIs.begin(); iter != ROIs.end(); iter++) {
		//returnString += "Index: " + str(iter->first) + ":\n";
		for (vector< ROI >::const_iterator ROIIter = iter->second.begin(); ROIIter != iter->second.end(); ROIIter++) {
			returnString += ROIIter->toString() + "\n";
		}
	}
	return returnString;

}

int ROIStringParser::getNumberOfROILines() const {
	return numberOfROILines;
}

int ROIStringParser::getNumberOfPeriods() const {
	return numberOfPeriods;
}

int ROIStringParser::getNumberOfRuns() const {
	return numberOfRuns;
}

int ROIStringParser::getNumberOfROIsForRunAndPeriod(int nRun, int nPeriod) const {

	int index = nRun*numberOfPeriods+nPeriod;
	map< int , vector< ROI > >::const_iterator iter = ROIs.find(index);
	if (iter == ROIs.end()) return 0;
	else return iter->second.size();

}

vector< ROI > ROIStringParser::getROIsForRunAndPeriod(int nRun, int nPeriod) const {

	int index = nRun*numberOfPeriods+nPeriod;
	map< int , vector< ROI > >::const_iterator iter = ROIs.find(index);
	if (iter == ROIs.end()) return vector< ROI >();
	else return iter->second;

}

