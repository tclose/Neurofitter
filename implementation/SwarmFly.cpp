#include "../SwarmFly.h"

ModelTuningParameters SwarmFly::bestGlobalSolution = ModelTuningParameters();
bool SwarmFly::bestGlobalInited = false;

void SwarmFly::setMembers(double newW, double newC, ModelTuningParameters startPoint, ModelTuningParameters startSpeed) {
        c = newC;
		w = newW;
		currentSpeed = startSpeed;
        setNewPositionFitness(startPoint);
}

ModelTuningParameters SwarmFly::calculateNewPosition() {

		bestInformantsSolution = bestLocalSolution;

		ModelTuningParameters informantsSolution;
		for (int i = 0; i < (int)informants.size(); i++) {
			informantsSolution = informants[i]->getBestLocalSolution();
			if (informantsSolution.getFitnessValue() < bestInformantsSolution.getFitnessValue()) 
				bestInformantsSolution = informantsSolution;
		}
		
        ModelTuningParameters newPosition(currentPosition);

        for (int i = 0; i < currentPosition.getLength(); i++) {
            currentSpeed[i] = w*currentSpeed[i] + randGen->rand(c)*(bestLocalSolution[i]-currentPosition[i]) + randGen->rand(c)*(bestInformantsSolution[i]-currentPosition[i]);
            newPosition[i] = currentPosition[i] + currentSpeed[i];
        }

		//Adapt speed and position to keep fly inside the bounds
		keepInBox(newPosition);

        return newPosition;
}


void SwarmFly::setNewPositionFitness(ModelTuningParameters & newPosition) {
       
	currentPosition = newPosition;
	double fitnessValue = currentPosition.getFitnessValue();

	showMessage("New position: " + currentPosition.toString() + " New Speed: " + currentSpeed.toString() + "\n", 4, fixedParams);

	if (!bestLocalInited || fitnessValue < bestLocalSolution.getFitnessValue()) {
		bestLocalInited = true;
		bestLocalSolution = currentPosition;
	}
	if (!bestGlobalInited || fitnessValue < bestGlobalSolution.getFitnessValue()) {
		bestGlobalInited = true;
		bestGlobalSolution = currentPosition;
	}
}

void SwarmFly::addInformant(SwarmFly * informant) {
	informants.push_back(informant);
}

void SwarmFly::resetInformants() {
	informants.clear();
}

ModelTuningParameters SwarmFly::getBestLocalSolution() {
	return bestLocalSolution;
}

vector< SwarmFly * > SwarmFly::getInformants() {
	return informants;
}

void SwarmFly::keepInBox(ModelTuningParameters & newPosition) {

	for (int i = 0; i < newPosition.getLength(); i++) {
		if (newPosition[i] > newPosition.getUpperBound(i)) {
			newPosition[i] = newPosition.getUpperBound(i);
			currentSpeed[i] = 0;
		}
		if (newPosition[i] < newPosition.getLowerBound(i)) {
			newPosition[i] = newPosition.getLowerBound(i);
			currentSpeed[i] = 0;
		}
	}
}
