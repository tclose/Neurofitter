/*
 Revision of last commit: $Rev$
 Author of last commit: $Author$
 Date of last commit: $Date$
 */

#include <math.h>
#include "../Tools.h"
#include "../NormalTracesReader.h"

/// Read data traces from a directory 
ModelResults NormalTracesReader::readTracesFromFilesVector(
    vector<string> filesVector) {

  int numberOfRuns = toInt(fixedParams["NumberOfRuns"]);

  // "results" will contain all the traces
  ModelResults results(
      getNumberOfRuns() * getNumberOfPeriods() * getNumberOfRecordSites());

  //////////////////////////////////////
  /// Read the weights for every run ///
  //////////////////////////////////////

  vector<double> runWeights(numberOfRuns, 0);
  double dummy;

  istringstream runStream(fixedParams["RunParameters"]);
  for (int i = 0; i < numberOfRuns; i++) {
    // Skip the run parameters
    for (int j = 0; j < getNumberOfRunParameters(); j++) {
      runStream >> dummy;
    }
    if (!runStream.good())
      crash("NormalTracesReader",
          "Error while reading " + str(numberOfRuns)
              + " run weights  from parameter file");
    runStream >> runWeights[i];
  }

  for (int nRun = 0; nRun < numberOfRuns; nRun++) {

    int numberOfRecordSites = toInt(fixedParams["NumberOfRecordSites"]);
    int numberOfPeriods = toInt(fixedParams["NumberOfPeriods"]);

    ///////////////////////////////////////////////
    /// Read the period start,stops and weights ///
    ///////////////////////////////////////////////
    vector<double> periodStart(numberOfPeriods, 0);
    vector<double> periodStops(numberOfPeriods, 0);
    vector<double> periodWeights(numberOfPeriods, 0);

    istringstream periodStream(fixedParams["Periods"]);
    for (int i = 0; i < numberOfPeriods; i++) {
      if (!periodStream.good())
        crash("NormalTracesReader",
            "Error while reading " + str(numberOfPeriods)
                + " recording periods from parameter file");
      periodStream >> periodStart[i];
      periodStream >> periodStops[i];
      if (periodStart[i] >= periodStops[i]) {
        crash("NormalTracesReader",
            "Error while reading " + str(numberOfPeriods)
                + " recording periods from parameter file: a start time is large than a stop time");
      }
      periodStream >> periodWeights[i];
    }

    vector<int> differenceLags(numberOfPeriods, 0);
    vector<double> differenceWeights(numberOfPeriods, 0);

    if (fixedParams.parameterExists("DifferenceLags")) {
      istringstream lagStream(fixedParams["DifferenceLags"]);
      for (int i = 0; i < numberOfPeriods; i++) {
        if (!lagStream.good())
          crash("NormalTracesReader",
              "Error while reading " + str(numberOfPeriods)
                  + " difference lags from parameter file");
        lagStream >> differenceLags[i];
        lagStream >> differenceWeights[i];
      }
    } else {
      for (int i = 0; i < numberOfPeriods; i++) {
        differenceLags[i] = 1;
        differenceWeights[i] = 0.5;
      }
    }

    ///////////////////////////////////////
    /// Read the recording site weights ///
    ///////////////////////////////////////
    vector<double> recordWeights(numberOfRecordSites, 0);
    istringstream recordStream(fixedParams["RecordSites"]);
    for (int i = 0; i < numberOfRecordSites; i++) {
      if (!recordStream.good())
        crash("NormalTracesReader",
            "Error while reading " + str(numberOfRecordSites)
                + " recording site weights from parameter file");
      recordStream >> recordWeights[i];
    }

    ////////////////////////////////////
    /// Read every period seperately ///
    ////////////////////////////////////
    for (int nPeriod = 0; nPeriod < numberOfPeriods; nPeriod++) {
      //Calculate the index of this period in the 1 dimensional array of traces
      int periodIndex = nRun * numberOfRecordSites * numberOfPeriods
          + nPeriod * numberOfRecordSites;

      //////////////////////////
      /// Open the data file ///
      //////////////////////////
      string inputFileName = filesVector[nRun];
      ifstream inputFile(inputFileName.c_str(), ios::in);
      if (!inputFile.good())
        crash("NormalTracesReader",
            "Error while opening file " + inputFileName);
      showMessage(
          "Reading from file: " + inputFileName + " from "
              + str(periodStart[nPeriod]) + " until "
              + str(periodStops[nPeriod]) + "\n", 5, fixedParams);

      ///////////////////////////////////////////////
      /// Read data until the start of the period ///
      ///////////////////////////////////////////////
      double time = 0;
      double dummy;
      inputFile >> time;
      while (time < periodStart[nPeriod]) {
        if (!inputFile.good())
          crash("NormalTracesReader",
              "Error while reading from file " + inputFileName + " at time "
                  + str(time));
        for (int nRecording = 0; nRecording < numberOfRecordSites;
            nRecording++) {
          inputFile >> dummy;
        }
        inputFile >> time;
      }

      /////////////////////////////
      /// Initialize the traces ///
      /////////////////////////////
      for (int nRecording = 0; nRecording < numberOfRecordSites; nRecording++) {
        results[periodIndex + nRecording].resetAndSetLength(
            (int) floor(
                periodStops[nPeriod]
                    * toDouble(fixedParams["SamplingFrequency"]))
                - (int) floor(
                    periodStart[nPeriod]
                        * toDouble(fixedParams["SamplingFrequency"])) + 1);
        results[periodIndex + nRecording].setName(
            "Run: " + str(nRun) + ", Period: " + str(nPeriod) + ", Recording: "
                + str(nRecording));
        results[periodIndex + nRecording].setSamplingFrequency(
            toDouble(fixedParams["SamplingFrequency"]));
        results[periodIndex + nRecording].setWeight(
            runWeights[nRun] * periodWeights[nPeriod]
                * recordWeights[nRecording]);
        results[periodIndex + nRecording].setStartTime(time);
        results[periodIndex + nRecording].setStopTime(periodStops[nPeriod]);
        results[periodIndex + nRecording].setLag(differenceLags[nPeriod],
            differenceWeights[nPeriod]);
        results[periodIndex + nRecording].setRun(nRun);
        results[periodIndex + nRecording].setNumberOfRuns(numberOfRuns);
        results[periodIndex + nRecording].setPeriod(nPeriod);
        results[periodIndex + nRecording].setNumberOfPeriods(numberOfPeriods);
        results[periodIndex + nRecording].setRecordingSite(nRecording);
        results[periodIndex + nRecording].setNumberOfRecordingSites(
            numberOfRecordSites);
      }

      /////////////////////
      /// Read the data ///
      /////////////////////
      int i = 0;
      double inputValue;
      while (time <= periodStops[nPeriod] && !inputFile.eof()) {
        if (!inputFile.good())
          crash("NormalTracesReader",
              "Error while reading from file " + inputFileName + " at time "
                  + str(time));
        for (int nRecording = 0; nRecording < numberOfRecordSites;
            nRecording++) {
          if (!inputFile.good())
            crash("NormalTracesReader",
                "Error while reading from file " + inputFileName + " at time "
                    + str(time));
          inputFile >> inputValue;
          results[periodIndex + nRecording].set(i, inputValue);
        }
        i = i + 1;
        inputFile >> time;
      }

      inputFile.close();
    }

  }

  return results;

}

ModelResults NormalTracesReader::readTracesFromDirectory(string dirName) {

  vector<string> filesVector(getNumberOfRuns());
  for (unsigned int i = 0; i < filesVector.size(); i++) {
    filesVector[i] = dirName + "/" + fixedParams["OutputFilePrefix"] + "_Run"
        + str(i) + ".dat";
  }
  return readTracesFromFilesVector(filesVector);
}

ModelResults NormalTracesReader::readTracesFromFilesList(string filesList) {

  vector<string> filesVector(getNumberOfRuns());
  istringstream fileNameStream(filesList);
  for (unsigned int i = 0; i < filesVector.size(); i++) {
    if (!fileNameStream.good())
      crash("NormalTracesReader",
          "Error while reading " + str(getNumberOfRuns())
              + " data filenames from XML settings file");
    char fileName[256];
    fileNameStream.getline(fileName, 256);
    string fileNameString(fileName);
    trimString(fileNameString);
    if (fileNameString == "")
      i = i - 1;
    else
      filesVector[i] = fileNameString;
  }
  return readTracesFromFilesVector(filesVector);
}

