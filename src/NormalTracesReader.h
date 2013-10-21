/*
Revision of last commit: $Rev: 241 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-22 16:17:53 +0900 (Mon, 22 Oct 2007) $
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
