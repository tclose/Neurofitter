#include "../EvolufitParameters.h"

EvolufitParameters::EvolufitParameters(EvolufitState * statePar) {

	verboseLevel = 0;
	state = statePar;

}


void EvolufitParameters::readFromFile(string filename) {

	cout << "Dummy reading Evolufit parameters from file: " << filename << "\n";

}

void EvolufitParameters::writeToFile(string filename) {

	cout << "Dummy writing Evolufit parameters to file: " << filename << "\n";

}

int EvolufitParameters::getVerboseLevel() const {

	return verboseLevel;

}
    
string EvolufitParameters::getDataFileLoc() const {

	return "You silly bastard";

}



