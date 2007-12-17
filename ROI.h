/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_ROI_H
#define NEUROFITTER_ROI_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

/// Class that parses the regions of interest settings in the XML file

class ROI {

	public:
		ROI() : sourceString("") {}; ///< Default constructor
		ROI(double minV, double maxV, double mindVdt, double maxdVdt, int vLength, int dVdtLength, double weight);
		ROI(const string vBoundsString, const string dVdtBoundsString, const string vdVdtLength, const string weight); ///< Constructor that uses an initializing string
		~ROI(); ///< Destructor

		string toString() const; ///< Returns a string representation
		int getVLength() const;
		int getdVdtLength() const;
		double getMinV() const;
		double getMaxV() const;
		double getMindVdt() const;
		double getMaxdVdt() const;
		double getWeight() const;

	private:
		string sourceString;
		double minV, maxV, mindVdt, maxdVdt, vLength, dVdtLength, weight;
};

#endif
