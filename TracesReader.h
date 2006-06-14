#ifndef NEUROFITTER_TRACEREADER_H
#define NEUROFITTER_TRACEREADER_H

#include <string>

#include "FixedParamObject.h"
#include "ModelResults.h"

using namespace std;

class TracesReader : public FixedParamObject {

	public:
		TracesReader(FixedParameters params) : FixedParamObject(params) {};
		virtual ModelResults readTraces(string dirName) = 0;
		virtual ~TracesReader() {};
		int getNumberOfRuns() {return toInt(fixedParams["NumberOfRuns"]);}
		int getNumberOfRunParameters() {return toInt(fixedParams["NumberOfRunParameters"]);}
		string getRunParameters() {return fixedParams["RunParameters"];}
		int getNumberOfRecordSites() {return toInt(fixedParams["NumberOfRecordSites"]);}
		int getNumberOfPeriods() {return toInt(fixedParams["NumberOfPeriods"]);}

		string getOutputFilePrefix() {return fixedParams["OutputFilePrefix"];}

	protected:

};

#endif
