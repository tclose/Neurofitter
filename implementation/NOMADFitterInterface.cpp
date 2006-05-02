#include "../NOMADFitterInterface.h"
#include <nomad>

NOMADFitterInterface::NOMADFitterInterface(FitnessCalculator * fit, FixedParameters params)
	: FitterInterface(fit), FixedParamObject(params) {

	setNOMADFitnessCalculator(fit, toInt(fixedParams["Dimensions"]));

}

FitterResults NOMADFitterInterface::runFitter(ModelTuningParameters * startPoints) {
	
	modelParams = startPoints;
      
	/////
	/// Writing the NOMAD input file
	////

	///todo change nomad to prevent LIBRARY directory
	string parametersFile="LIBRARY/parameters.dat";
	string descriptionFile="LIBRARY/description.dat";
	string preferencesFile="LIBRARY/preferences.dat";

	writeStartingPointsFile(fixedParams["START_PT_FILE"], *startPoints);
	writeBoundsFile(fixedParams["BOUNDS_FILE"], *startPoints);  
	writeParametersFile(parametersFile);
	writeDescriptionFile(descriptionFile);
	writePreferencesFile(preferencesFile);

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

extern string NOMADDescContents;

void NOMADFitterInterface::writeDescriptionFile(string fileName) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	file << "DIMENSION\t"<< fixedParams["Dimensions"] << endl;

	file << "PROBLEM_NAME\t"<< "Evolufit" << endl;
	file << "SCALING_CHOICE\t"<< fixedParams["SCALING_CHOICE"] << endl;
	file << "GEN_CONS_NB\t"<< fixedParams["GEN_CONS_NB"] << endl;
	file << "USE_SURROGATE\t"<< fixedParams["USE_SURROGATE"] << endl;
	
	file << "USE_BLACK_BOXES\t"<< "0" << endl;
	file << "USE_BOUNDS\t"<< "1" << endl;
	file << "USE_CACHES\t"<< "0" << endl;
	file << "DIRECTORY\t"<< "." << endl;

	file << "START_PT_FILE\t"<< fixedParams["START_PT_FILE"] << endl;
	file << "BOUNDS_FILE\t"<< fixedParams["BOUNDS_FILE"] << endl;
	file << "INPUT_FILE\t"<< fixedParams["INPUT_FILE"] << endl;
	file << "RESULTS_FILE\t"<< fixedParams["RESULTS_FILE"] << endl;
	
	file.close();
}

void NOMADFitterInterface::writeParametersFile(string fileName) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

	file << "RANDOMSEED/t"<< toInt(fixedParams["Seed"]) << endl;	

	file << "INITIAL_POLL_SIZE\t" << fixedParams["INITIAL_POLL_SIZE"] << endl;	
	file << "MAX_POLL_SIZE\t" << fixedParams["MAX_POLL_SIZE"] << endl;	
	file << "POLL_BASIS/t"<< fixedParams["POLL_BASIS"] << endl;	
	file << "COARSENING_EXPONENT"<< fixedParams["COARSENING_EXPONENT"] << endl;	
	file << "REFINING_EXPONENT"<< fixedParams["REFINING_EXPONENT"] << endl;
	
	file << "POLL_ORDER"<< fixedParams["POLL_ORDER"] << endl;	
	file << "POLL_COMPLETE"<< fixedParams["POLL_COMPLETE"] << endl;	
	file << "POLL_DIRECTIONS"<< fixedParams["POLL_DIRECTIONS"] << endl;
	
	file << "INITIAL_SEARCH"<< fixedParams["INITIAL_SEARCH"] << endl;	
	file << "INITIAL_COMPLETE"<< fixedParams["INITIAL_COMPLETE"] << endl;	
	file << "INITIAL_POINTS"<< fixedParams["INITIAL_POINTS"] << endl;	

	file << "ITERATIVE_SEARCH"<< fixedParams["ITERATIVE_SEARCH"] << endl;	
	file << "ITERATIVE_COMPLETE"<< fixedParams["ITERATIVE_COMPLETE"] << endl;	
	file << "ITERATIVE_POINTS"<< fixedParams["ITERATIVE_POINTS"] << endl;	

	file << "SPECULATIVE_SEARCH"<< fixedParams["SPECULATIVE_SEARCH"] << endl;	

	file << "POLL_SIZE_TERM"<< fixedParams["POLL_SIZE_TERM"] << endl;	
	file << "CONSECUTIVE_FAILS"<< fixedParams["CONSECUTIVE_FAILS"] << endl;	
	file << "TRUTH_EVALS"<< fixedParams["TRUTH_EVALS"] << endl;	
	file << "ITERATIONS"<< fixedParams["ITERATIONS"] << endl;	
	file << "NEW_TRUTH_EVALS"<< fixedParams["NEW_TRUTH_EVALS"] << endl;	

	file << "FILTER_RELATIVE"<< fixedParams["FILTER_RELATIVE"] << endl;	
	file << "HMAX"<< fixedParams["HMAX"] << endl;	
	file << "HMIN"<< fixedParams["HMIN"] << endl;	
	file << "FILTER_NORM"<< fixedParams["FILTER_NORM"] << endl;
	
	file << "SURROGATE_TOLERANCE"<< fixedParams["SURROGATE_TOLERANCE"] << endl;	

	file.close();

}

void NOMADFitterInterface::writePreferencesFile(string fileName) {

	ofstream file;
	file.open(fileName.c_str(), ios::out);

    file << "DISPLAY_FACTOR\t " << fixedParams["VerboseLevel"] << endl;
    file << "SEND_EMAIL      0" << endl;

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



