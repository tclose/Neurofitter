/*
Revision of last commit: $Rev$
Author of last commit: $Author$
Date of last commit: $Date$
*/

#include "../DiffEvolutionFitterInterface.h"

FitterResults DiffEvolutionFitterInterface::runFitter(ModelTuningParameters * startPoint) {

	vector< vector< vector < int > > > recombinationIndices(
			numberOfIndividuals, 
			vector< vector< int > > (
				numberOfIndividuals, vector < int > (numberOfIndividuals,  0)
			)
	);  
	
	vector< ModelTuningParameters >  currentGeneration(numberOfIndividuals, *startPoint);
	vector< ModelTuningParameters >  newGeneration(numberOfIndividuals, *startPoint);


	makeNewCombinations(recombinationIndices);
	newGeneration = recombine(currentGeneration, recombinationIndices);

	vector< ModelTuningParameters > results;

	return FitterResults(results);
}

void makeNewCombinations(vector< vector< vector < int > > > & recombinationIndices) {


}

vector< ModelTuningParameters > recombine(vector< ModelTuningParameters > currentGeneration, vector< vector< vector < int > > > recombinationIndices) {



}



