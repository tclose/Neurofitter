#ifndef NEUROFITTER_NORMALTRACEREADER_H
#define NEUROFITTER_NORMALTRACEREADER_H

#include <string>
#include <fstream>

#include "ModelResults.h"
#include "TracesReader.h"

class NormalTracesReader : public TracesReader {

	public:
		NormalTracesReader(FixedParameters params) : TracesReader(params) {};
		ModelResults readTraces(string dirName);

	protected:

};

#endif
