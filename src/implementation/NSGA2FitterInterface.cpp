/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../NSGA2FitterInterface.h"

NSGA2FitterInterface::NSGA2FitterInterface(ErrorValueCalculator * fit, FixedParameters params) : FitterInterface(fit), FixedParamObject(params), parentPopulationSize(toUnsigned(fixedParams["ParentPopulationSize"]))  {

	if (fixedParams.parameterExists("ExportFile")) {
        this->enableFileExport(fixedParams["ExportFile"]);
    }

}


FitterResults NSGA2FitterInterface::runFitter(ModelTuningParameters * startPoint) {

	MTRand randGen( toInt(fixedParams["Seed"]) );

	population=NSGA2Population(&randGen,fixedParams);
	parents=NSGA2Population(&randGen, fixedParams);
	children=NSGA2Population(&randGen, fixedParams);

	parents.initialize(*startPoint, parentPopulationSize);
	population.initialize(*startPoint, 0);
	children.initialize(*startPoint, parentPopulationSize);

	unsigned generationNumber = 0;

	while(true) {
		population = parents.makeUnion(children);
		population.declassify();

		population.calculateErrorValues(errorValue);
		population.classify();

		writeToExportFile(generationNumber);	

		parents.clear();
		
		int rank = 0;
		while ((population.getFront(rank)).size() != 0 && parents.getSize() + (population.getFront(rank)).size() <= parentPopulationSize) {
			population.calculateFrontCrowdingDistance(rank);
			parents = parents.makeUnion(population.getFront(rank));
			rank++;
		}

		population.calculateFrontCrowdingDistance(rank);		
		vector< NSGA2Individual > sortedFront = population.getSortedFront(rank);
		sortedFront.erase(sortedFront.begin()+(parentPopulationSize-parents.getSize()),sortedFront.end());
		parents = parents.makeUnion(sortedFront);
		children = parents.createChildren();
	
		generationNumber++;
	}

	return FitterResults();
 
}

void NSGA2FitterInterface::writeToExportFile(unsigned generationNumber) {

	if (exportFileStream.is_open()) {
			vector< NSGA2Individual > individuals = population.getIndividuals();
			ModelTuningParameters params;
			for (unsigned i = 0; i < individuals.size(); i++) {
            	exportFileStream << generationNumber << " " << individuals[i].getRank() << " ";
            	
				for (unsigned j = 0; j < individuals[i].getNumberOfObjectives(); j++) {
                	exportFileStream << individuals[i].getObjective(j) << " ";
            	}

				params = individuals[i].getModelTuningParameters();
				for (unsigned j = 0; j < params.getLength(); j++) {
                	exportFileStream << params[j] << " ";
            	}

            	exportFileStream << endl;
			}
        }


}

void NSGA2FitterInterface::enableFileExport(const string fileName) {
    exportFileStream.open(fileName.c_str(), ios::out);

    showMessage("NSGA2FitterInterface: Enabled export to file: " + fileName + "\n",3,fixedParams);

}

