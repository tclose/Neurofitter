#ifndef NEUROFITTER_EODISTFITNESS_h
#define NEUROFITTER_EODISTFITNESS_h

#include <queue>
#include <algorithm>

#include <eoPop.h>
#include <eoPopEvalFunc.h>

#include "FitnessCalculator.h"

template <class EOT> class EODistFitness : public eoPopEvalFunc <EOT> {
  
public :
  
  /** Constructor */
  EODistFitness(FitnessCalculator * f) : fitness(f) {};
  
  /** For a population of parents
      and children */
  	void operator () (eoPop <EOT> & __parents, eoPop <EOT> & __offspring) {
    	/* Notes. '_parents' is discarded ! (See. TimeVarying ...)
       	Individuals in a population will be mixed */

		vector< ModelTuningParameters > paramList(__offspring.size());


    	for (int i = 0; i < (int)__offspring.size(); i++) {
			paramList[i] = ModelTuningParameters(__offspring[i].size());
			for (int j = 0; j < (int)__offspring[i].size(); j++) {
				paramList[i][j] = __offspring[i][j];
			}
    	}

    	for (int i = 0; i < (int)__offspring.size(); i++) {
			__offspring[i].fitness(paramList[i].getFitnessValue());
		}

	}
  
private :
	
	FitnessCalculator * fitness; 

} ;

#endif
