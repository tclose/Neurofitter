#ifndef EVOLUFITSOLVER_H
#define EVOLUFITSOLVER_H

class EvolufitSolver {

	virtual EvolufitResults solve(EvolufitParameters*, ModelInterface*, FitterInterface**, FitnessCalculator*);

}

#endif
