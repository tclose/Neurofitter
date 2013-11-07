/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#ifndef NEUROFITTER_NORMALTRACEREADER_H
#define NEUROFITTER_NORMALTRACEREADER_H

#include <string>
#include <fstream>

#include "ModelResults.h"
#include "TracesReader.h"

/// The standard reader of traces from files
class NormalTracesReader : public TracesReader {

	public:
		NormalTracesReader(FixedParameters params) : TracesReader(params) {}; ///< Constructor that reads the fixed parameters
		virtual ModelResults readTracesFromDirectory(string dirName); /// Read the traces from a directory
		virtual ModelResults readTracesFromFilesList(string filesList); /// Read the traces from a list of filenames, filenames are seperated by newlines
	protected:
		virtual ModelResults readTracesFromFilesVector(vector< string > filesVector);
};

#endif
