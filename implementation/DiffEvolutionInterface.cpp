/*
Revision of last commit: $Rev: 227 $
Author of last commit: $Author: werner $
Date of last commit: $Date: 2007-10-18 13:29:41 +0900 (Thu, 18 Oct 2007) $
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



