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
		ROIStringParser(const string, int newNumberOfROIs, int newNumberOfRuns, int newNumberOfPeriods); ///< Constructor that uses an initializing string
		~ROIStringParser(); ///< Destructor

		string toString() const; ///< Returns a string representation
		int getNumberOfROIs() const;
		int getNumberOfROILines() const;
		int getNumberOfPeriods() const;
		int getNumberOfRuns() const;
		int getNumberOfROIsForRunAndPeriod(int nRun, int nPeriod) const;
		vector< ROI > getROIsForRunAndPeriod(int nRun, int nPeriod) const;

	private:
		string sourceString;
		int numberOfROILines;
		int numberOfRuns;
		int numberOfPeriods;
		int numberOfROIs;
		map< int , vector< ROI > > ROIs;
};

#endif
