#ifndef EVOLUFIT_STATE_H
#define EVOLUFIT_STATE_H

using namespace std;

class EvolufitState {

public:
	EvolufitState();
	void setVerboseLevel(const int newLevel);
	int getVerboseLevel() const;

private:
	int verboseLevel;

};

#endif


