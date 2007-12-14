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
		ROI(double minV, double maxV, double minVdVdt, double maxVdVdt, double weight);
		ROI(const string vBoundsString, const string dVdtBoundsString, const string weight); ///< Constructor that uses an initializing string
		~ROI(); ///< Destructor

		string toString() const; ///< Returns a string representation
		double getMinV() const;
		double getMaxV() const;
		double getMinVdVdt() const;
		double getMaxVdVdt() const;
		double getWeight() const;

	private:
		string sourceString;
		double minV, maxV, minVdVdt, maxVdVdt, weight;
};

#endif
