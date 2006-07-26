#ifndef NEUROFITTER_TRACEREADER_H
#define NEUROFITTER_TRACEREADER_H

#include <string>

#include "FixedParamObject.h"
#include "ModelResults.h"

using namespace std;

/// Abstract class that reads data traces from files
class TracesReader : public FixedParamObject {

	public:
		TracesReader(FixedParameters params) : FixedParamObject(params) {}; ///< Constructor which read fixed parameters
		virtual ModelResults readTraces(string dirName) = 0; ///< Read the traces from a certain directory
		virtual ~TracesReader() {}; ///< Destructor
		int getNumberOfRuns() {return toInt(fixedParams["NumberOfRuns"]);} ///< Get the number of runs that is being read
		int getNumberOfRunParameters() {return toInt(fixedParams["NumberOfRunParameters"]);} ///< Get the number of parameters that is being read
		string getRunParameters() {return fixedParams["RunParameters"];} ///< Get the parameters for every run
		int getNumberOfRecordSites() {return toInt(fixedParams["NumberOfRecordSites"]);} ///< Get the number of number of recording sites
		int getNumberOfPeriods() {return toInt(fixedParams["NumberOfPeriods"]);} ///< Get the number of periods

		string getOutputFilePrefix() {return fixedParams["OutputFilePrefix"];} ///< Get the file prefix of the files being read

	protected:

};

#endif
