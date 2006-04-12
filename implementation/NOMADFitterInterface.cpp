#include "../NOMADFitterInterface.h"
#include <nomad>

FitterResults NOMADFitterInterface::runFitter(ModelTuningParameters * startPoints, int seed) {
	
	modelParams = startPoints;
      
	/////
	/// Writing the NOMAD input file
	////
	string startingPointsFile="start_pt.txt";
	string boundsFile="bounds.txt";
	string parametersFile="LIBRARY/parameters.dat";
	string descriptionFile="LIBRARY/description.dat";
	string preferencesFile="LIBRARY/preferences.dat";

	writeStartingPointsFile(startingPointsFile, *startPoints);
	writeBoundsFile(boundsFile, *startPoints);  
	writeParametersFile(parametersFile, seed);
	//writeDescriptionFile(descriptionFile);
	//writePreferencesFile(preferencesFile);

	/////
	///	Creating and initiliaze NOMAD objects
	////
    Parameters parameters(parametersFile.c_str());
    Description description(descriptionFile.c_str());
	//todo ask NOMAD developer to allow location preferences file
    Preferences preferences;
    Solver solver;

    if (!description.readyToSolve()) {cerr << endl << "Error while processing NOMAD description file" << endl;exit(1);}
	description.capture(solver);


	/////
	/// Solve the problem
	////
	solveProblem(description, parameters, preferences, solver);

	return FitterResults();

}


void NOMADFitterInterface::writeStartingPointsFile(string fileName, ModelTuningParameters & startPts) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	for (int i = 0; i < startPts.getLength(); i++) {
		file << startPts[i] << " ";
	}

	file.close();

}

void NOMADFitterInterface::writeBoundsFile(string fileName, ModelTuningParameters & startPts) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	for (int i = 0; i < startPts.getLength(); i++) {
		file << startPts.getLowerBound(i) << " " << endl;
	}

	for (int i = 0; i < startPts.getLength(); i++) {
		file << startPts.getUpperBound(i) << " " << endl;
	}

	file.close();

}

extern string NOMADParamsContents;

void NOMADFitterInterface::writeParametersFile(string fileName, int seed) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	file << endl << "	RANDOMSEED		"<< seed << endl;	

	file << NOMADParamsContents;

	file.close();

}

void NOMADFitterInterface::solveProblem(const Description & description,
          const Parameters & parameters, Preferences & preferences,
          Solver & solver) {

	if (parameters.readyToSolve()) {
		parameters.capture(description.getGeneralConstraintsNumber(),
		description.getSurrogateUse(), solver);

		preferences.capture(description.getName(), solver);

		if (solver.evaluateStartingPoint() > 0) {
			ofstream fout(description.getResultsFile());
			solver.showInput(fout);
			solver.solveProblem(fout);

			fout.close();

			preferences.endOfRun();
    	}
		else {
      		cerr << "The starting point is outside the bounds!\n";
      		cerr << "Please correct before running NOMAD.\n";
      		cerr << "See the program documentation in the 'DOC' directory.\n\n";
		}
  	}
  	else {   // We alert the user that something's wrong with the parameters.
		cerr << "Something is wrong with the NOMAD parameters!\n";
		cerr << "Please correct before running NOMAD.\n";
		 cerr << "See the program documentation in the 'DOC' directory.\n\n";
	}

}


string NOMADParamsContents = " \
	INITIAL_POLL_SIZE   10000 \n\
	MAX_POLL_SIZE       100000 \n\
	POLL_BASIS      	4 \n\
	COARSENING_EXPONENT 1 \n\
	REFINING_EXPONENT   -1 \n\
\n\
	RANDOM_SEED			65 \n\
\n\
	POLL_ORDER      	0 \n\
	POLL_COMPLETE   	0 \n\
	POLL_DIRECTIONS 	3 \n\
\n\
	INITIAL_SEARCH      1 \n\
	INITIAL_COMPLETE    0 \n\
	INITIAL_POINTS      20 \n\
	ITERATIVE_SEARCH    1 \n\
	ITERATIVE_COMPLETE  0 \n\
	ITERATIVE_POINTS    20 \n\
	SPECULATIVE_SEARCH  1 \n\
\n\
	POLL_SIZE_TERM      -1 \n\
	CONSECUTIVE_FAILS   -1 \n\
	TRUTH_EVALS     1000 \n\
	ITERATIONS      -1 \n\
	NEW_TRUTH_EVALS     -1 \n\
\n\
	FILTER_RELATIVE     1 \n\
	HMAX            	50000 \n\
	HMIN            	0.0001 \n\
	FILTER_NORM     	2 \n\
\n\
	SURROGATE_TOLERANCE 0";

