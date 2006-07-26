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
		virtual ModelResults readTraces(string dirName); /// Read the traces from a directory

	protected:

};

#endif
