#include <string>
#include <sstream>

#include "../ROI.h"
#include "../Tools.h"
#include "../ROIStringParser.h"

ROIStringParser::~ROIStringParser() {}

ROIStringParser::ROIStringParser(const string newString, int newNumberOfROILines, int newNumberOfRuns, int newNumberOfPeriods, int newNumberOfRecordingSites) {

	initialize(newString, newNumberOfROILines, newNumberOfRuns, newNumberOfPeriods, newNumberOfRecordingSites);

}


void ROIStringParser::initialize(const string newString, int newNumberOfROILines, int newNumberOfRuns, int newNumberOfPeriods, int newNumberOfRecordingSites) {

	ROIs.clear();
	sourceString=newString;
	numberOfROILines = newNumberOfROILines;
	numberOfRuns = newNumberOfRuns;
	numberOfPeriods = newNumberOfPeriods;
	numberOfRecordingSites = newNumberOfRecordingSites;
	numberOfROIs = 0;

	string dummy, allRunStr, allPeriodStr, allRecordStr, runStr, periodStr, recordStr, vBoundsStr, dVdtBoundsStr, lengthsStr, overflowStr, weightStr;
	int run, period, record;

	istringstream ss(sourceString);

	for (int i = 0; i < numberOfROILines; i++) {
		if (!ss.good()) crash("ROIStringParser", "Unable to read from ROI string\n");
		getline(ss, dummy, '['); 
		getline(ss, allRunStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, allPeriodStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, allRecordStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, vBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, dVdtBoundsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, lengthsStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, overflowStr, ']');
		getline(ss, dummy, '['); 
		getline(ss, weightStr, ']');

		if (!ss.good()) crash("ROIStringParser", "Unable to read from ROI string\n");

		istringstream allRunStream(allRunStr);
		istringstream allPeriodStream(allPeriodStr);
		istringstream allRecordStream(allRecordStr);

		vector< int > runVector;
		vector< int > periodVector;
		vector< int > recordVector;

		while (allRunStream.good()) {
			getline(allRunStream,runStr,',');
			istringstream runStream(runStr);
			runStream >> run;
			if (run < 0 || run >= numberOfRuns) crash("ROIStringParser","Run with number " + str(run) + " does not exist");
			runVector.push_back(run);
		}
		while (allPeriodStream.good()) {
			getline(allPeriodStream,periodStr,',');
			istringstream periodStream(periodStr);
			periodStream >> period;
			if (period < 0 || period >= numberOfPeriods) crash("ROIStringParser","Period with number " + str(period) + " does not exist");
			periodVector.push_back(period);
		}
		while (allRecordStream.good()) {
			getline(allRecordStream,recordStr,',');
			istringstream recordStream(recordStr);
			recordStream >> record;
			if (record < 0 || record >= numberOfRecordingSites) crash("ROIStringParser","Recording site with number " + str(record) + " does not exist");
			recordVector.push_back(record);
		}

		if (!ss.good()) crash("ROIStringParser", "Unable to read from ROI string\n");

		for (unsigned int nRun = 0; nRun < runVector.size(); nRun++) {
			for (unsigned int nPeriod = 0; nPeriod < periodVector.size(); nPeriod++) {
				for (unsigned int nRecord = 0; nRecord < recordVector.size(); nRecord++) {
					ROI roi(vBoundsStr, dVdtBoundsStr, lengthsStr, overflowStr, weightStr);
					ROIs[runVector[nRun]*numberOfPeriods*numberOfRecordingSites+periodVector[nPeriod]*numberOfRecordingSites+recordVector[nRecord]].push_back(roi);
					numberOfROIs++;
				}
			}
		}	
		
	}

}




string ROIStringParser::toString() const {
	string returnString ="";
	returnString += "Number of runs: " + str(numberOfRuns) + "\tNumber of periods: " + str(numberOfPeriods) + "\tNumber of recording sites: " + str(numberOfRecordingSites) + "\tNumber of ROIs: " + str(numberOfROIs) + "\n";
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

int ROIStringParser::getNumberOfRecordingSites() const {
	return numberOfRecordingSites;
}

int ROIStringParser::getNumberOfRuns() const {
	return numberOfRuns;
}

int ROIStringParser::getNumberOfROIsForRunAndPeriodAndRecord(int nRun, int nPeriod, int nRecord) const {

	int index = nRun*numberOfPeriods*numberOfRecordingSites+nPeriod*numberOfRecordingSites+nRecord;
	map< int , vector< ROI > >::const_iterator iter = ROIs.find(index);
	if (iter == ROIs.end()) return 0;
	else return iter->second.size();

}

vector< ROI > ROIStringParser::getROIsForRunAndPeriodAndRecord(int nRun, int nPeriod, int nRecord) const {

	int index = nRun*numberOfPeriods*numberOfRecordingSites+nPeriod*numberOfRecordingSites+nRecord;
	map< int , vector< ROI > >::const_iterator iter = ROIs.find(index);
	if (iter == ROIs.end()) return vector< ROI >();
	else return iter->second;

}

