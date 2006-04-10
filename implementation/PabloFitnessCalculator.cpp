#include "../PabloFitnessCalculator.h"

PabloFitnessCalculator::PabloFitnessCalculator(const ModelInterface * interface, const ExperimentInterface * experiment) {
	model = interface;
	ModelResults expData = experiment->getData();
	
	expVdVdtMatrices = new PabloVdVdtMatrix [expData.getLength()]; 
	for (int nTrace = 0; nTrace < expData.getLength(); nTrace++) {
		expVdVdtMatrices[nTrace] =  PabloVdVdtMatrix(expData[nTrace]);
	}
}

PabloFitnessCalculator::~PabloFitnessCalculator() {
	delete [] expVdVdtMatrices;
}

double PabloFitnessCalculator::calculateFitness(const ModelTuningParameters & params) const {

	double fitnessValue = 0;

	ModelResults results = model->runModel(params);

    for (int nTrace = 0; nTrace < results.getLength(); nTrace++) {
		PabloVdVdtMatrix modelVdVdtMatrix(results[nTrace]);
		fitnessValue += results[nTrace].getWeight() * expVdVdtMatrices[nTrace].compare(modelVdVdtMatrix);
    }

	cout << "Calculated fitness of: " << params.toString() << ": " << fitnessValue << endl;
	
	return fitnessValue;
}

