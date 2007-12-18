/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_ROISTRINGPARSER_H
#define NEUROFITTER_ROISTRINGPARSER_H

#include <vector>
#include <string>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "ROI.h"

using namespace std;

/// Class that parses the regions of interest settings in the XML file

class ROIStringParser {

	public:
		ROIStringParser() : sourceString("") {}; ///< Default constructor
		ROIStringParser(const string, int newNumberOfROIs, int newNumberOfRuns, int newNumberOfPeriods, int newNumberOfRecordingSites); ///< Constructor that uses an initializing string
		~ROIStringParser(); ///< Destructor

		void initialize(const string, int newNumberOfROIs, int newNumberOfRuns, int newNumberOfPeriods, int newNumberOfRecordingSites);
		string toString() const; ///< Returns a string representation
		int getNumberOfROIs() const;
		int getNumberOfROILines() const;
		int getNumberOfPeriods() const;
		int getNumberOfRecordingSites() const;
		int getNumberOfRuns() const;
		int getNumberOfROIsForRunAndPeriodAndRecord(int nRun, int nPeriod, int nRecord) const;
		vector< ROI > getROIsForRunAndPeriodAndRecord(int nRun, int nPeriod, int nRecord) const;

	private:
		string sourceString;
		int numberOfROILines;
		int numberOfRuns;
		int numberOfPeriods;
		int numberOfRecordingSites;
		int numberOfROIs;
		map< int , vector< ROI > > ROIs;
};

#endif
