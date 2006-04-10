#include "../EvolufitState.h"

EvolufitState::EvolufitState() {
	
	verboseLevel = 0;

}

int EvolufitState::getVerboseLevel() const {

	return verboseLevel;

}

void EvolufitState::setVerboseLevel(const int newLevel) {

	verboseLevel = newLevel;

}

