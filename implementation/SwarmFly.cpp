#include "../SwarmFly.h"


void SwarmFly::setMembers(FitnessCalculator *fit, double newC1, double newC2, ModelTuningParameters startPoint, ModelTuningParameters startSpeed) {
        fitness = fit;
        c1 = newC1;
        c2 = newC2;
		currentSpeed = startSpeed;
        move(startPoint);
}

void SwarmFly::fly() {
        ///todo is this necessary ???
        ModelTuningParameters newPosition(currentPosition);
        ModelTuningParameters newSpeed(currentPosition.getLength());

        double r1 = randGen->rand();
        double r2 = randGen->rand();

        for (int i = 0; i < currentPosition.getLength(); i++) {
            newSpeed[i] = inertial*currentSpeed[i] +
                              c1*r1*(bestLocalSolution[i]-currentPosition[i]) +
                              c2*r2*(bestGlobalSolution[i]-currentPosition[i]);
            currentSpeed = newSpeed;
            newPosition[i] = currentPosition[i] + currentSpeed[i];
        }
        move(newPosition);
}


void SwarmFly::move(ModelTuningParameters position) {
       
	currentPosition = position;
	double fitnessValue = fitness->calculateFitness(currentPosition);

	cout << "Current speed: " << currentSpeed.toString() << endl;


	if (fitnessValue < bestLocalFitnessValue) {
		bestLocalFitnessValue = fitnessValue;
		bestLocalSolution = position;
	}
	if (fitnessValue < bestGlobalFitnessValue) {
		bestGlobalFitnessValue = fitnessValue;
		bestGlobalSolution = position;
	}
}


double SwarmFly::bestGlobalFitnessValue = 10000000;
double SwarmFly::inertial = 0.9;
ModelTuningParameters SwarmFly::bestGlobalSolution = ModelTuningParameters();
