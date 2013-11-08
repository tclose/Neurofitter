/*
Revision of last commit: $Rev: 223 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-04 14:08:47 +0900 (Thu, 04 Oct 2007) $
*/

#include "../SwarmFly.h"

ModelTuningParameters SwarmFly::bestGlobalSolution = ModelTuningParameters();
bool SwarmFly::bestGlobalInited = false;


SwarmFly::SwarmFly(double newW, double newC, MTRand * rand, FixedParameters params) :
        FixedParamObject(params),
        currentPosition(toInt(fixedParams["Dimensions"]), fixedParams["Bounds"]),
        currentSpeed(toInt(fixedParams["Dimensions"])),
        c(newC), w(newW),
        randGen(rand),
        bestLocalInited(false), nextPositionRandom(false) {}


ModelTuningParameters SwarmFly::calculateNewPosition() {

		if (nextPositionRandom) {
			nextPositionRandom = false;
			return calculateRandomPosition();
		}
		else {
			bestInformantsSolution = bestLocalSolution;

			ModelTuningParameters informantsSolution;
			for (int i = 0; i < (int)informants.size(); i++) {
				informantsSolution = informants[i]->getBestLocalSolution();
				if (informantsSolution.getErrorValue() < bestInformantsSolution.getErrorValue()) 
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
}


void SwarmFly::setNewPositionErrorValue(ModelTuningParameters & newPosition) {
       
	currentPosition = newPosition;
	double errorValue = currentPosition.getErrorValue();

	showMessage("New position: " + currentPosition.toString() + " New Speed: " + currentSpeed.toString() + "\n", 4, fixedParams);

	if (!bestLocalInited || errorValue < bestLocalSolution.getErrorValue()) {
		bestLocalInited = true;
		bestLocalSolution = currentPosition;
	}
	if (!bestGlobalInited || errorValue < bestGlobalSolution.getErrorValue()) {
		bestGlobalInited = true;
		bestGlobalSolution = currentPosition;
	}
}

ModelTuningParameters SwarmFly::calculateRandomPosition() {

	ModelTuningParameters randomPosition(currentPosition);
	randomPosition.resetErrorValue();
	
	for (int i = 0; i < randomPosition.getLength(); i++) {
		randomPosition[i] = randomPosition.getLowerBound(i)+randGen->rand(randomPosition.getUpperBound(i)-randomPosition.getLowerBound(i));
    }
	for (int i = 0; i < currentSpeed.getLength(); i++) {
		double startX = randomPosition.getLowerBound(i)+randGen->rand(randomPosition.getUpperBound(i)-randomPosition.getLowerBound(i));
		double startY = randomPosition.getLowerBound(i)+randGen->rand(randomPosition.getUpperBound(i)-randomPosition.getLowerBound(i));
        currentSpeed[i] = startX-startY;
    }

	//Adapt speed and position to keep fly inside the bounds (at this point not really necessary, but just checking)
	keepInBox(randomPosition);

	return randomPosition;
}

ModelTuningParameters & SwarmFly::getParameters() {	
	return currentPosition;
}

double SwarmFly::getErrorValue() {
	return currentPosition.getErrorValue();
}

void SwarmFly::addInformant(SwarmFly * informant) {
	informants.push_back(informant);
}

void SwarmFly::resetInformants() {
	informants.clear();
}

ModelTuningParameters SwarmFly::getBestLocalSolution() {
	if (bestLocalInited) return bestLocalSolution;
	else crash("SwarmFly","Getting uninitialized local solution");
	//Should not be reached
	return -1;
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

void SwarmFly::makeNextPositionRandom() {
	nextPositionRandom = true;
}
